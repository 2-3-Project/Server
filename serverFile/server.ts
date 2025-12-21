import express, { Request, Response } from "express";
import { spawn } from "child_process";

import db from "./db";

const app = express();
app.use(express.json());

app.post("/auth", (req : Request, res : Response) => {
  const { name } = req.body;
  if(!name){
      return res.status(400).send("이름을 입력해주세요!");
  }

  try{
      db.prepare(`
          INSERT INTO users (name) VALUES (?)
      `).run(name);

      res.send("회원가입이 완료됨!");
  }
  catch (err){
      res.status(400).send("이미 존재하는 아이디 입니다!");
  }
});

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


app.listen(3000, () => {
  console.log("Server running on http://localhost:3000");
});
=======
import SpeechRecognition, {useSpeechRecognition} from "react-speech-recognition";

const useSpeechToText = () => {
  const {
    transcript,
    listening,
    browserSupportsSpeechRecognition
  } = useSpeechRecognition();

  const toggleListening = () => {
    if (!browserSupportsSpeechRecognition) {
      alert("이 브라우저는 음성 인식을 지원하지 않습니다.");
      return;
    }

    if (listening) {
      SpeechRecognition.stopListening();
    } else {
      SpeechRecognition.startListening({
        language: "ko-KR",
        continuous: true,
        interimResults: true
      });
    }
  };

  return {
    transcript,
    listening,
    toggleListening,
    browserSupportsSpeechRecognition
  };
};

export default useSpeechToText;
