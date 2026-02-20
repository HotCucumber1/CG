class HangmanModel {
    constructor() {
        this._tries = 7
    }

    isAlive() {
        return this._tries > 0
    }

    get tries() {
        return this._tries
    }

    decreaseTries() {
        this._tries--
    }
}
