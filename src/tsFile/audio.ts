// 주문 리스트
const spells = [
  "치링치링 샤랄라 나날이 예뻐지는 나. 너무나도 소중해!",
  "사랑의 멋짐을 모르는 당신은 불쌍해요..!",
  "인간이 다섯 명이나 모이면 말야... 반드시 한 명 쓰레기가 있다",
  "샤랄라 꿈꿔 왔던 내 모습 마법 소녀로 변신",
  "후루룩 짭짭 펑! 푸딩 폭풍 스윗 푸딩 블라스트",
  "착한 마음으로 물들어라",
  "거대한 탐욕이여, 너의 본색을 드러내라!",
  "잃어버린 양심을 되찾아 줄 희망의 빛이여!",
  "화염을 부르는 마법! 파이너셜 파이어스톰 테러!",
  "니코니코 웃음, 마음을 환하게! 러브 스마일, 러브 웨이브!",
  "혼란의 마음에 고요한 안식을 소울 캄 캡슐 라이트!",
  "빛나는 별의 장막을 펼쳐줄래? 하이 톤 갤럭시 스크린!"
];

// 1차 : 콘솔용 볼륨 체크
navigator.mediaDevices.getUserMedia({ audio: true })
  .then(stream => {
    const audioContext = new AudioContext();
    const source = audioContext.createMediaStreamSource(stream);
    const analyser = audioContext.createAnalyser();

    source.connect(analyser);

    const dataArray = new Uint8Array(analyser.fftSize);

    function checkVolume() {
      analyser.getByteTimeDomainData(dataArray);
      let sum = 0;

      for (let i = 0; i < dataArray.length; i++) {
        sum += Math.abs(dataArray[i] - 128);
      }

      const volume = sum / dataArray.length;

      // 기준값 예시
      if (volume < 40 || volume > 85) console.log("실패");
      else if (volume >= 40 && volume <= 69) console.log("미흡");
      else if (volume >= 70 && volume <= 85) console.log("성공");
      else console.log("volume 오류");

      requestAnimationFrame(checkVolume);
    }

    checkVolume();
  });

// 2차 : 실제 게임용 볼륨 + 데미지 계산
import SpeechRecognition, { useSpeechRecognition } from "react-speech-recognition";

let damage = 0;
let volume: number = 0;

navigator.mediaDevices.getUserMedia({ audio: true })
  .then(stream => {
    const audioContext = new AudioContext();
    const source = audioContext.createMediaStreamSource(stream);
    const analyser = audioContext.createAnalyser();

    source.connect(analyser);
    const dataArray = new Uint8Array(analyser.fftSize);

    function checkVolume() {
      analyser.getByteTimeDomainData(dataArray);

      let sum = 0;
      for (let i = 0; i < dataArray.length; i++) {
        sum += Math.abs(dataArray[i] - 128);
      }

      volume = sum / dataArray.length;

      // 데미지 1차 판정 받기 전이기 때문에 데시벨 기준에 맞는 기본값만 계산
      // 실제 최종 데미지는 finalScore
      if (volume < 40 || volume > 85) damage = 0;
      else if (volume >= 40 && volume <= 69) damage = 10000;
      else if (volume >= 70 && volume <= 85) damage = 15000;
      else damage = 0;

      requestAnimationFrame(checkVolume);
    }

    checkVolume();
  });


const spellData = {
  spellId: "SPELL_001",
  pronunciation: volume,
  volume: damage,
};

export default spellData;

// 음성 인식 hook
interface UseSpeechToTextResult {
  transcript: string;
  listening: boolean;
  toggleListening: () => void;
  browserSupportsSpeechRecognition: boolean;
}

const useSpeechToText = (): UseSpeechToTextResult => {
  const {
    transcript,
    listening,
    browserSupportsSpeechRecognition
  } = useSpeechRecognition();

  const toggleListening = (): void => {
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

export const text = useSpeechToText;

// 문자 단위 정확도
function accuracyPercent(target: string, result: string): number {
  if (!target || !result) return 0;
  const minLen = Math.min(target.length, result.length);
  let same = 0;

  for (let i = 0; i < minLen; i++) {
    if (target[i] === result[i]) same++
  }

  return (same / target.length) * 100 // 퍼센트 계산
}

// 단어 단위 정확도
function wordAccuracy(target: string, result: string) : number {
  const tWords = target.trim().split(/\s+/);
  const rWords = result.trim().split(/\s+/);

  if(tWords.length === 0) return 0;

  let same = 0;
  tWords.forEach((w, idx) => {
    if(rWords[idx] && rWords[idx] === w) same++;
  });

  return (same / tWords.length) * 100;
}

export function finalScore(target: string, result: string): number {
  const charScore = accuracyPercent(target, result);
  const wordScore = wordAccuracy(target, result);

  return Math.round(charScore * 0.7 + wordScore * 0.3);
}

// 판정 - JSON 타입
export type FirstJudgeResult = "성공" | "실패";
export type SecondJudgeResult = "성공" | "미흡" | "실패";

export interface SpellJson {
  spellId : string;
  target : string; // 주문 원문
  result : string; // 인식된 문장
  finalScore : number; // 1차 점수
  firstJudge : FirstJudgeResult; // 1차 결과
  secondJudge : SecondJudgeResult; // 2차 결과
  decibel : number; // 데시벨
  damage : number; // 최종 데미지
}

// 최종 판정 - JSON 생성 함수
export function createSpellJson(target : string, result : string) : SpellJson {
  // 1차 발음 정확도
  const score = finalScore(target, result);
  
  let firstJudge : FirstJudgeResult = "실패";
  let secondJudge : SecondJudgeResult = "실패";
  let finalDamage = 0;

  // 1차 판정 : 정확도 70% 이상이면 성공
  if(score >= 70) {
    firstJudge = "성공";
  }

  // 2차 판정 : 데시벨 표
  // 40dB 미만 -> 실패 / 데미지 0
  // 40dB ~ 69dB -> 미흡 / 기본 공격 10,000
  // 70dB ~ 85dB -> 성공 / 강공격 15,000
  // 86dB 초과 -> 실패 / 데미지 0
  if(volume < 40) {
    secondJudge = "실패";
    finalDamage = 0;
  } else if(volume > 85) {
    secondJudge = "실패";
    finalDamage = 0;
  } else if(volume >= 70 && volume <= 85) {
    secondJudge = "성공";
    finalDamage = 15000;
  } else if(volume >= 40 && volume <= 69) {
    secondJudge = "미흡";
    finalDamage = 10000;
  } else {
    // 1차 실패면 바로 실패 처리(damage = 0)
    firstJudge = "성공";
    secondJudge = "실패";
    finalDamage = 0;
  }

  return {
    spellId: "SPELL_001",
    target, // 주문 원문
    result, // 음성 인식 결과 텍스트
    finalScore : score,
    firstJudge,
    secondJudge,
    decibel : volume, // 위에서 계속해서 갱신되는 전역 volume 값을 사용
    damage : finalDamage
  };
}