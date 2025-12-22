import DataBase from "better-sqlite3";

const db = new DataBase("game.db");

db.pragma("foreign_keys = ON");

db.prepare(`
    CREATE TABLE IF NOT EXISTS users (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        name TEXT NOT NULL UNIQUE,
        rank INTEGER,
        hp REAL
    )
`).run();

db.prepare(`
    CREATE TABLE IF NOT EXISTS ranking (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        user_id INTEGER NOT NULL,
        name TEXT,
        hp REAL,
        FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE
    )
`).run();

export default db;