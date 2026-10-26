#!/usr/bin/env python3
"""RecordBase backend server (Python 3, stdlib only).

Serves the records database over a small JSON REST API and hosts the
frontend in web/. Data lives in server/db.sqlite3, seeded on first run
from data/catalogs.csv, datasets.csv, entries.csv, transactions.csv.

Run:  python backend/server.py   ->  http://localhost:8080
"""

import csv
import json
import os
import sqlite3
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DATA = os.path.join(ROOT, "data")
WEB = os.path.join(ROOT, "web")
DB_PATH = os.path.join(ROOT, "backend", "db.sqlite3")

SCHEMA = """
CREATE TABLE IF NOT EXISTS catalog (
  catalog_id INTEGER PRIMARY KEY AUTOINCREMENT,
  catalogname TEXT NOT NULL UNIQUE,
  description TEXT
);
CREATE TABLE IF NOT EXISTS dataset (
  dataset_id TEXT PRIMARY KEY,
  catalog_id INTEGER NOT NULL REFERENCES catalog(catalog_id) ON DELETE CASCADE,
  date_of_dataset TEXT, source_table TEXT, target_table TEXT,
  start_time TEXT, end_time TEXT, status TEXT DEFAULT 'PENDING',
  total_rows INTEGER DEFAULT 0, rows_indexed INTEGER DEFAULT 0
);
CREATE TABLE IF NOT EXISTS entry (
  entry_id TEXT PRIMARY KEY,
  name_suffix TEXT, name TEXT, address TEXT, city TEXT,
  gender TEXT, data_of_birth TEXT, weight TEXT
);
CREATE TABLE IF NOT EXISTS transaction_tbl (
  transaction_id TEXT PRIMARY KEY,
  entry_id TEXT NOT NULL REFERENCES entry(entry_id),
  dataset_id TEXT NOT NULL REFERENCES dataset(dataset_id),
  amount REAL DEFAULT 0
);
CREATE TABLE IF NOT EXISTS index_key (
  key_id INTEGER PRIMARY KEY AUTOINCREMENT,
  dataset_id TEXT NOT NULL REFERENCES dataset(dataset_id) ON DELETE CASCADE,
  index_key TEXT NOT NULL, entry_id TEXT, available INTEGER DEFAULT 1,
  amount REAL DEFAULT 0
);
"""


def seed(conn):
    """Load the CSV dataset from data/ into SQLite (idempotent)."""
    if conn.execute("SELECT COUNT(*) FROM catalog").fetchone()[0] > 0:
        return
    with open(os.path.join(DATA, "catalogs.csv"), newline="", encoding="utf-8") as f:
        for row in csv.DictReader(f):
            conn.execute("INSERT INTO catalog(catalogname, description) VALUES (?, ?)",
                         (row["catalogname"], row.get("description", "")))
    cat_ids = {r[0]: r[1] for r in conn.execute("SELECT catalog_id, catalogname FROM catalog")}
    with open(os.path.join(DATA, "datasets.csv"), newline="", encoding="utf-8") as f:
        for r in csv.DictReader(f):
            conn.execute(
                "INSERT INTO dataset VALUES (?,?,?,?,?,?,?,?,?,?)",
                (r["dataset_id"], cat_ids.get(r["catalogname"], 1), r["date_of_dataset"],
                 r["source_table"], r["target_table"], r["start_time"], r["end_time"],
                 r.get("status", "PENDING"),
                 int(r["total_rows"]), int(r["rows_indexed"])))
    with open(os.path.join(DATA, "entries.csv"), newline="", encoding="utf-8") as f:
        for r in csv.DictReader(f):
            conn.execute(
                "INSERT INTO entry VALUES (?,?,?,?,?,?,?,?)",
                (r["entry_id"], r["name_suffix"], r["name"], r["address"], r["city"],
                 r["gender"], r["data_of_birth"], r["weight"]))
    with open(os.path.join(DATA, "transactions.csv"), newline="", encoding="utf-8") as f:
        for r in csv.DictReader(f):
            conn.execute("INSERT INTO transaction_tbl VALUES (?,?,?,?)",
                         (r["transaction_id"], r["entry_id"], r["dataset_id"],
                          float(r["amount"])))
    with open(os.path.join(DATA, "indexes.csv"), newline="", encoding="utf-8") as f:
        for r in csv.DictReader(f):
            conn.execute("INSERT INTO index_key(dataset_id,index_key,entry_id,available,amount) "
                         "VALUES (?,?,?,?,?)",
                         (r["dataset_id"], r["index_key"], r["entry_id"],
                          int(r["available"] or 0), float(r["amount"] or 0)))
    conn.commit()


def get_db():
    conn = sqlite3.connect(DB_PATH)
    conn.row_factory = sqlite3.Row
    return conn


def reset_db():
    """Delete the SQLite file so the next start re-seeds from data/*.csv."""
    if os.path.exists(DB_PATH):
        os.remove(DB_PATH)


class Handler(BaseHTTPRequestHandler):
    def _send(self, code, body, ctype="application/json"):
        data = body if isinstance(body, bytes) else json.dumps(body).encode()
        self.send_response(code)
        self.send_header("Content-Type", ctype)
        self.send_header("Content-Length", str(len(data)))
        self.send_header("Access-Control-Allow-Origin", "*")
        self.end_headers()
        self.wfile.write(data)

    def _json_body(self):
        n = int(self.headers.get("Content-Length") or 0)
        return json.loads(self.rfile.read(n) or b"{}")

    # ---------------- routes ----------------
    def do_GET(self):
        path = self.path.split("?")[0]
        conn = get_db()
        try:
            if path == "/api/catalogs":
                rows = conn.execute("SELECT * FROM catalog").fetchall()
                self._send(200, [dict(r) for r in rows])
            elif path == "/api/datasets":
                rows = conn.execute(
                    "SELECT d.*, c.catalogname FROM dataset d "
                    "JOIN catalog c ON c.catalog_id=d.catalog_id "
                    "ORDER BY d.start_time").fetchall()
                self._send(200, [dict(r) for r in rows])
            elif path == "/api/entries":
                rows = conn.execute("SELECT * FROM entry ORDER BY name").fetchall()
                self._send(200, [dict(r) for r in rows])
            elif path == "/api/transactions":
                rows = conn.execute(
                    "SELECT t.*, e.name FROM transaction_tbl t "
                    "JOIN entry e ON e.entry_id=t.entry_id").fetchall()
                self._send(200, [dict(r) for r in rows])
            elif path.startswith("/api/transactions/") and path.endswith("/tree"):
                tid = path.split("/")[-2]
                tr = conn.execute("SELECT * FROM transaction_tbl WHERE transaction_id=?",
                                  (tid,)).fetchone()
                if not tr:
                    return self._send(404, {"error": "not found"})
                keys = conn.execute(
                    "SELECT * FROM index_key WHERE dataset_id=?",
                    (tr["dataset_id"],)).fetchall()
                self._send(200, {"transaction": dict(tr),
                                 "index_keys": [dict(r) for r in keys]})
            elif path == "/api/stats":
                s = {}
                for t in ("catalog", "dataset", "entry", "transaction_tbl", "index_key"):
                    s[t] = conn.execute(f"SELECT COUNT(*) FROM {t}").fetchone()[0]
                s["amount_total"] = conn.execute(
                    "SELECT COALESCE(SUM(amount),0) FROM transaction_tbl").fetchone()[0]
                self._send(200, s)
            else:
                self._static(path)
        finally:
            conn.close()

    def do_POST(self):
        path = self.path.split("?")[0]
        conn = get_db()
        try:
            body = self._json_body()
            if path == "/api/transactions":
                try:
                    conn.execute("INSERT INTO transaction_tbl VALUES (?,?,?,?)",
                                 (body["transaction_id"], body["entry_id"],
                                  body["dataset_id"], float(body.get("amount", 0))))
                    conn.commit()
                except sqlite3.IntegrityError as e:
                    return self._send(409, {"error": str(e)})
                return self._send(201, {"ok": True})
            self._send(404, {"error": "unknown endpoint"})
        finally:
            conn.close()

    def do_DELETE(self):
        path = self.path.split("?")[0]
        conn = get_db()
        try:
            if path.startswith("/api/transactions/"):
                tid = path.rsplit("/", 1)[-1]
                cur = conn.execute("DELETE FROM transaction_tbl WHERE transaction_id=?",
                                   (tid,))
                conn.commit()
                if cur.rowcount:
                    return self._send(200, {"ok": True})
                return self._send(404, {"error": "not found"})
            self._send(404, {"error": "unknown endpoint"})
        finally:
            conn.close()

    def _static(self, path):
        if path in ("/", "/index.html"):
            fname = "index.html"
        else:
            fname = path.lstrip("/")
        full = os.path.join(WEB, fname)
        if not os.path.isfile(full):
            return self._send(404, {"error": "not found"})
        ctype = "text/html" if fname.endswith(".html") else \
            "application/javascript" if fname.endswith(".js") else \
            "text/css" if fname.endswith(".css") else "text/plain"
        with open(full, "rb") as f:
            self._send(200, f.read(), ctype)


def main():
    os.makedirs(os.path.dirname(DB_PATH), exist_ok=True)
    reset_db()
    conn = sqlite3.connect(DB_PATH)
    conn.executescript(SCHEMA)
    seed(conn)
    conn.close()
    print("RecordBase backend running on http://localhost:8080")
    ThreadingHTTPServer(("0.0.0.0", 8080), Handler).serve_forever()


if __name__ == "__main__":
    main()
