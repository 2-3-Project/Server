import express, { Request, Response } from "express";
import cors from "cors";
import { spawn } from "child_process";

import db from "./db";

const app = express();
app.use(express.json());
app.use(cors());

interface UserRow{
  id: number;
  name: string;
  hp: number;
}

//회원가입
app.post("/auth", (req : Request, res : Response) => {
  const { name } = req.body;
  if(!name){
      return res.status(400).send("이름을 입력해주세요!");
  }
  else{
    try{
      const nameExists = (name: string): boolean => {
        const stmt = db.prepare(`
          SELECT 1 FROM users WHERE name = ?
        `);
        
        return !!stmt.get(name);
      };
      if(nameExists(name)){
        res.send("이미 존재하는 이름입니다.");
      }
      else{
        db.prepare(`
        INSERT INTO users (name) VALUES (?)
        `).run(name);

        res.send("회원가입이 완료됨!");
      }
    }
    catch (err){
      res.status(400).send("이미 존재하는 아이디 입니다!");
    }
  }


});

//json c++에 보내기
app.post("/game/attack", (req : Request, res : Response) => {
  const getJson = req.body;

  const cpp = spawn("src/main/main.exe");

  let output = "";

  cpp.stdin.write(JSON.stringify(getJson));
  cpp.stdin.end();

  cpp.stdout.on("data", (data) => {
    output += data.toString();
  });

  cpp.stderr.on("data", (err) => {
    console.error("C++ error : ", err.toString());
  });
  
  cpp.on("close", () => {
    res.send(output.trim());
  });

});

app.post("/clear", (req: Request, res: Response) => {
  const {name, hp} = req.body;

  try{
    const resultData = db
    .prepare(`UPDATE users SET hp = ? WHERE name = ?`)
    .run(hp, name);

    if(resultData.changes === 0){
      return res.status(404).json({message: "유저 찾지 못함."});
    }
    res.json({message: "HP 업데이트 완료"});
  } catch(err){
    console.error(err);
    res.status(500).json({error: "DB 오류"});
  }
});

app.post("/calc/ranking", (req: Request, res: Response) => {
  try{
    const users = db.prepare(`
      SELECT id, name, hp
      FROM users
      ORDER BY hp DESC
    `).all() as UserRow[];

    db.prepare(`DELETE FROM ranking`).run();

    let rank = 0;
    let prevHp: number | null = null;
    let saveRank = 0;
    
    const insert = db.prepare(`
      INSET INTO ranking (userId, name, rank, hp) VALUES (?, ?, ?, ?)
    `);

    for(const user of users){
      saveRank++;
      if(prevHp === null || user.hp < prevHp){
        rank = saveRank;
      }
      insert.run(user.id, user.name, rank, user.hp);
      prevHp = user.hp;
    }
    res.json({message: "랭킹 계산 완료"});
  } catch(err){
    console.error(err);
    res.status(500).json({error: "랭킹 계산 오류"});
  }
});

app.get("/ranking", (req: Request, res: Response) => {
  try{
    const data = db.prepare(`
      SELECT rank, name, hp FROM ranking ORDER BY rank DESC
    `).all();

    res.json({data});
  } catch(err) {
    console.error(err);
    res.status(500).json({error: "랭킹 조회 오류"});
  }
});


app.listen(3000, () => {
  console.log("Server running");
});