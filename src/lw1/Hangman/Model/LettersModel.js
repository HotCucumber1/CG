const LetterState = Object.freeze({
    Default: 'Default',
    Correct: 'Correct',
    Incorrect: 'Incorrect',
});

class LettersModel {
    /**
     * @param {string} letters
     */
    constructor(letters) {
        this._clickedLetters = []
        this._letters = new Map()
        this._initLetters(letters)
    }

    chooseLetter(letter) {
        if (this._clickedLetters.includes(letter)) {
            return
        }
        this._clickedLetters.push(letter)
    }

    /**
     *
     * @param {string} letter
     * @param {LetterState} state
     */
    setLetterState(letter, state) {
        if (!this._letters.has(letter)) {
            throw new Error(`Wrong letter: ${letter}`)
        }
        this._letters.set(letter, state)
    }

    get letters() {
        return this._letters;
    }

    /**
     *
     * @param {string} letters
     * @private
     */
    _initLetters(letters) {
        Array.from(letters).forEach((letter) => {
            this._letters.set(letter, LetterState.Default)
        })
    }
}
