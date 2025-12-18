import SpeechRecognition, { useSpeechRecognition } from "react-speech-recognition";

let damage = 0;
let volume;
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

      // 기준값 예시
      if (volume < 40 || volume > 85) damage = 0;
      else if (volume >= 40 && volume <= 69 ) damage = 10000;
      else if (volume >= 70 && volume <=85) damage =  15000;
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
export const Data = spellData;


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