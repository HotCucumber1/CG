class WordModel {
    /**
     * @param {string} word
     * @param {string} topic
     */
    constructor(word, topic) {
        this._word = word.toUpperCase()
        this._topic = topic.toUpperCase()
        this._lettersStatus = new Map()
        this._initWordStatus()
    }

    get word() {
        return this._word
    }

    get topic() {
        return this._topic
    }

    get lettersStatus() {
        return this._lettersStatus
    }

    get isWordOpen() {
        let status = true
        this._lettersStatus.forEach((isOpen) => {
            if (!isOpen) {
                status = false
            }
        })
        return status
    }

    /**
     * @param {string} letter
     */
    openLetter(letter) {
        if (!this._lettersStatus.has(letter)) {
            throw new Error(`Wrong letter: ${letter}`)
        }
        this._lettersStatus.set(letter, true)
    }

    _initWordStatus() {
        Array.from(this._word).forEach((letter) => {
            this._lettersStatus.set(letter, false)
        })
    }
}
