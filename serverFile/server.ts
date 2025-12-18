import { spawn } from "child_process";
import express, { Request, Response } from "express";
import {Data} from "../src/tsFile/audio";

const app = express();

app.post("/sign", (req: Request, res: Response) => {
  return null;
});

app.get("/start", (req: Request, res: Response) => {
  const cpp = spawn("src/main/main.exe");

  cpp.stdout.on("data", (outputData) => {
    console.log(outputData.toString());
  });

  cpp.stdin.on("data", (inputData) => {
    console.log(inputData);
  });

  cpp.on("close", () => {
    res.send("C++ 실행 완료");
  });
});

app.listen(3000, () => {
  console.log("Server running on http://localhost:3000/");
});