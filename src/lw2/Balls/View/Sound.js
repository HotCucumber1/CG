class SoundManager {
    constructor(url) {
        this._globalVolume = 1;
        const audio = new Audio(url);
        audio.volume = this._globalVolume;
        this._sound = audio;
    }

    play() {
        if (this._sound) {
            this._sound.currentTime = 0;
            this._sound.play()
                .catch(error => {
                    console.log('Failed to play sound:', error);
                });
        }
    }
}
