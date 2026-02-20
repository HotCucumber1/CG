class HangmanView {
    /**
     * @param {CanvasRenderingContext2D} context
     * @param {HangmanModel} model
     * @param {Object} appSettings
     */
    constructor(context, model, appSettings) {
        this._context = context
        this._model = model
        this._appSettings = appSettings

        this._gallowsTopLeft = {
            x: 100,
            y: 100,
        }
        this._gallowsWidth = 10
        this._gallowsHeight = 500
        this._manBodyX = this._gallowsTopLeft.x + this._gallowsHeight / 2

        this._ropeLength = 150
        this._bodyLength = 150
        this._limbWidth = 7
        this._headR = 40
    }

    draw() {
        this._drawGallows()
        this._drawMan()
    }

    drawLose() {
        alert("You lose")
        window.location.reload()
    }

    drawVictory() {
        alert("You won!")
        window.location.reload()
    }

    _drawGallows() {
        this._context.fillStyle = "green"
        this._context.fillRect(this._gallowsTopLeft.x, this._gallowsTopLeft.y, this._gallowsWidth, this._gallowsHeight)
        this._context.fillRect(this._gallowsTopLeft.x, this._gallowsTopLeft.y, this._gallowsHeight / 2, this._gallowsWidth)
    }

    _drawMan() {
        const tries = this._model.tries

        this._context.strokeStyle = "black"
        this._context.fillStyle = "black"
        if (tries < 7) {
            this._drawRope()
        }
        if (tries < 6) {
            this._drawHead()
        }
        if (tries < 5) {
            this._drawBody()
        }
        if (tries < 4) {
            this._drawLeftArm()
        }
        if (tries < 3) {
            this._drawRightArm()
        }
        if (tries < 2) {
            this._drawLeftLeg()
        }
        if (tries < 1) {
            this._drawRightLeg()
        }
    }

    _drawRope() {
        const ropeWidth = 2
        this._drawLine(
            this._manBodyX,
            this._gallowsTopLeft.y,
            this._manBodyX,
            this._gallowsTopLeft.y + this._ropeLength,
            ropeWidth,
        )
    }

    _drawHead() {
        this._context.beginPath()
        this._context.arc(this._manBodyX, this._gallowsTopLeft.y + this._ropeLength, this._headR, 0, 2 * Math.PI)
        this._context.fill()
    }

    _drawBody() {
        const bodyWidth = 10
        this._drawLine(
            this._manBodyX,
            this._gallowsTopLeft.y + this._ropeLength,
            this._manBodyX,
            this._gallowsTopLeft.y + this._ropeLength + this._bodyLength,
            bodyWidth,
        )
    }

    _drawLeftArm() {
        this._drawLine(
            this._manBodyX,
            this._gallowsTopLeft.y + this._ropeLength + this._headR,
            this._manBodyX - 100,
            this._gallowsTopLeft.y + this._ropeLength + 100,
            this._limbWidth,
        )
    }

    _drawRightArm() {
        this._drawLine(
            this._manBodyX,
            this._gallowsTopLeft.y + this._ropeLength + this._headR,
            this._manBodyX + 100,
            this._gallowsTopLeft.y + this._ropeLength + 100,
            this._limbWidth,
        )
    }

    _drawLeftLeg() {
        this._drawLine(
            this._manBodyX,
            this._gallowsTopLeft.y + this._ropeLength + this._bodyLength,
            this._manBodyX - 50,
            this._gallowsTopLeft.y + this._ropeLength + this._bodyLength + 100,
            this._limbWidth,
        )
    }

    _drawRightLeg() {
        this._drawLine(
            this._manBodyX,
            this._gallowsTopLeft.y + this._ropeLength + this._bodyLength,
            this._manBodyX + 50,
            this._gallowsTopLeft.y + this._ropeLength + this._bodyLength + 100,
            this._limbWidth,
        )
    }

    _drawLine(x1, y1, x2, y2, width) {
        this._context.lineWidth = width
        this._context.beginPath()
        this._context.moveTo(x1, y1)
        this._context.lineTo(x2, y2)
        this._context.stroke()
    }
}
