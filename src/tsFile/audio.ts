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
      if (volume < 40 || volume > 85) console.log("🔈 실패");
      else if (volume >= 40 && volume <= 69 ) console.log("🔊 미흡");
      else if (volume >= 70 && volume <=85) console.log("성공");
      else console.log("volume 오류");

      requestAnimationFrame(checkVolume);
    }

    checkVolume();
  });
