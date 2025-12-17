const { spawn } = require("child_process");

const cpp = spawn(
  "src\\main\\main.exe"
);

cpp.stdout.on("data", data => {
  console.log("C++:", data.toString());
});

cpp.stderr.on("data", data => {
  console.error("C++ ERR:", data.toString());
});

cpp.on("error", err => {
  console.error("실행 실패:", err.message);
});

const spellData = {
  spellId: "SPELL_001",
  pronunciation: 87.3,
  volume: 55,
};

// 반드시 개행 포함
cpp.stdin.write(JSON.stringify(spellData) + "\n");










