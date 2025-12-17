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
