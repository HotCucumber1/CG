class LettersView {
    /**
     *
     * @param {CanvasRenderingContext2D} context
     * @param {LettersModel} model
     */
    constructor(context, model) {
        this._size = 40

        this._context = context
        this._letters = model.letters
        this._lettersView = new Map()
        this._initLettersView()
    }

    draw() {
        const defaultColor = 'black'
        const correctColor = 'green'
        const incorrectColor = 'red'

        this._context.font = `${this._size}px Arial`
        this._letters.forEach((state, letter) => {
            let color
            switch (state) {
                case LetterState.Default:
                    color = defaultColor
                    break
                case LetterState.Correct:
                    color = correctColor
                    break
                case LetterState.Incorrect:
                    color = incorrectColor
                    break
                default:
                    throw new Error("Undefined letter state " + state)
            }

            const letterViewInfo = this._lettersView.get(letter)
            this._context.fillStyle = color
            this._context.fillText(letter, letterViewInfo.x, letterViewInfo.y, letterViewInfo.x)
        })
    }

    onClick(callback) {
        window.addEventListener('click', callback)
    }

    onMouseMove(isMouseOnLetter) {
        window.addEventListener('mousemove', (event) => {
            if (isMouseOnLetter(event.clientX, event.clientY)) {
                document.body.style.cursor = 'pointer'
            }
            else {
                document.body.style.cursor = 'default'
            }
        })
    }

    _initLettersView() {
        const startX = 100
        const y = 700

        this._letters.keys().forEach((letter, index) => {
            this._lettersView.set(letter, {
                x: startX + this._size * index,
                y: y,
                width: this._size,
                height: this._size,
            })
        })
    }
}
