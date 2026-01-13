import express, { Request, Response } from "express";
import cors from "cors";
import { spawn } from "child_process";
import fs from "fs";
import path from "path";
import cors from "cors";
// import db from "./db";

const app = express();
app.use(cors());
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
  console.log("Received JSON:", getJson);

  // JSON을 input.json에 저장
  const inputPath = path.join(process.cwd(), "src/game/input/input.json");
  fs.writeFileSync(inputPath, JSON.stringify(getJson, null, 2));

  const cpp = spawn("./main.exe");
  console.log("Spawning C++");

  let output = "";

  cpp.stdout.on("data", (data) => {
    console.log("C++ stdout:", data.toString());
    output += data.toString();
  });

  cpp.stderr.on("data", (err) => {
    console.log("C++ stderr:", err.toString());
  });
  
  cpp.on("close", (code) => {
    console.log("C++ closed with code:", code);
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

  console.log("Server running on http://localhost:3000");
}).on('error', (err) => {
  console.error('Server failed to start:', err.message);
  process.exit(1);
});

// 루트 경로 추가
app.get("/", (req: Request, res: Response) => {
  res.send("서버가 실행 중입니다. /game/attack 엔드포인트로 POST 요청을 보내세요.");
});