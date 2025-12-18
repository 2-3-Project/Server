import DataBase from "better-sqlite3";

const db = new DataBase("game.db");

db.pragma("foreign_keys = ON");

db.prepare(`
    CREATE TABLE IF NOT EXISTS users (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        name TEXT NOT NULL,
        rank INTEGER,
        hp REAL,
    )
`).run();

db.prepare(`
    CREATE TABLE IF NOT EXISTS ranking (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        name TEXT,
        hp REAL,
        rank INTEGER,
        FOREIGN KEY (name) REFENCES users(name) ON DELETE CASCADE,
        FOREIGN KEY (rank) REFENCES users(rank) ON DELETE CASCADE
    )
`).run();

export default db;