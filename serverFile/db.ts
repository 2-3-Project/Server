import DataBase from "better-sqlite3";
import path from "path";

const dbPath = path.join(__dirname, "../game.db");

const db = new DataBase(dbPath);

db.pragma("foreign_keys = ON");

db.prepare(`
    CREATE TABLE IF NOT EXISTS users (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        name TEXT NOT NULL UNIQUE,
        rank INTEGER DEFAULT 0,
        hp REAL
    )
`).run();

db.prepare(`
    CREATE TABLE IF NOT EXISTS ranking (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        userId INTEGER NOT NULL,
        FOREIGN KEY (userID) REFERENCES users(id) ON DELETE CASCADE
    )
`).run();

export default db;