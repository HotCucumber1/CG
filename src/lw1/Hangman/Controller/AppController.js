class AppController {
    /**
     * @param {LettersModel} lettersModel
     * @param {HangmanModel} hangmanModel
     * @param {WordModel} wordModel
     * @param {LettersView} lettersView
     * @param {HangmanView} hangmanView
     * @param {WordView} wordView
     */
    constructor(
        lettersModel,
        hangmanModel,
        wordModel,
        lettersView,
        hangmanView,
        wordView,
    ) {
        this._size = 40
        this._lettersModel = lettersModel
        this._hangmanModel = hangmanModel
        this._wordModel = wordModel
        this._lettersView = lettersView
        this._hangmanView = hangmanView
        this._wordView = wordView
        this._lettersData = new Map()

        this._initLettersView()
        this._initEventsListeners()
    }

    drawGame() {
        this._hangmanView.draw()
        this._lettersView.draw()
        this._wordView.draw()
    }

    isAlive() {
        return this._hangmanModel.isAlive()
    }

    isVictory() {
        return this._wordModel.isWordOpen
    }

    drawVictory() {
        this._hangmanView.drawVictory()
    }

    drawLose() {
        this._hangmanView.drawLose()
    }

    _initEventsListeners() {
        this._lettersView.onClick((event) => this._handleMouseClick(event))
        this._lettersView.onMouseMove((x, y) => {
            return this._tryGetClickedLetter(x, y) !== null
        })
    }

    _handleMouseClick(event) {
        const clickedLetter = this._tryGetClickedLetter(
            event.clientX,
            event.clientY,
        )
        if (clickedLetter === null) {
            return
        }

        this._lettersModel.chooseLetter(clickedLetter)
        const word = this._wordModel.word

        let status
        if (word.includes(clickedLetter)) { // TODO вынести в модель
            status = LetterState.Correct
            this._wordModel.openLetter(clickedLetter)
        } else {
            status = LetterState.Incorrect
            this._hangmanModel.decreaseTries()
        }
        this._lettersModel.setLetterState(clickedLetter, status)
    }

    _tryGetClickedLetter(x, y) {
        let clickedLetter = null
        this._lettersData.forEach((data, letter) => {
            if (data.x <= x && x <= data.x + data.width &&
                data.y <= y && y <= data.y + data.height
            ) {
                clickedLetter = letter
            }
        })
        return clickedLetter
    }

    _initLettersView() {
        const startX = 100
        const y = 700
        const letters = this._lettersModel.letters

        letters.keys().forEach((letter, index) => {
            this._lettersData.set(letter, {
                x: startX + this._size * index,
                y: y - this._size,
                width: this._size,
                height: this._size,
            })
        })
    }
}
