import express, { Request, Response } from "express";
import { spawn } from "child_process";
import fs from "fs";
import path from "path";
import cors from "cors";
// import db from "./db";

const app = express();
app.use(cors());
app.use(express.json());

//회원가입
// app.post("/auth", (req : Request, res : Response) => {
//   const { name } = req.body;
//   if(!name){
//       return res.status(400).send("이름을 입력해주세요!");
//   }

//   //변경 사항
//   try{
//       db.prepare(`
//           INSERT INTO users (name) VALUES (?)
//       `).run(name);

//       res.send("회원가입이 완료됨!");
//   }
//   catch (err){
//       res.status(400).send("이미 존재하는 아이디 입니다!");
//   }
// });

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