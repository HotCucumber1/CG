class WordView {
    /**
     * @param {CanvasRenderingContext2D} context
     * @param {WordModel} model
     */
    constructor(context, model) {
        this._model = model
        this._context = context
        this._platformColor = "blue"
        this._letterColor = "black"
    }

    draw() {
        this._drawWord()
        this._drawTopic()
    }

    _drawWord() {
        const letterSize = 40
        const spacing = 20
        const startX = 700
        const y = 300

        const word = this._model.word
        const lettersStatus = this._model.lettersStatus

        Array.from(word).forEach((letter, i) => {
            this._context.fillStyle = this._platformColor
            this._context.fillRect(startX + letterSize * i + spacing * i, y, letterSize, 5)
            const isOpen = lettersStatus.get(letter)

            if (isOpen) {
                this._context.fillStyle = this._letterColor
                this._context.fillText(letter, startX + letterSize * i + spacing * i, y - 5, letterSize)
            }
        })
    }

    _drawTopic() {
        const topicStartPosition = {
            x: 700,
            y: 100,
        }

        this._context.fillStyle = this._letterColor
        this._context.fillText(
            this._model.topic,
            topicStartPosition.x,
            topicStartPosition.y,
        )
    }
}
