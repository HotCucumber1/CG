class Lines {
    constructor(y, space, width, context) {
        this._y = y
        this._width = width
        this._space = space
        this._context = context
    }

    draw() {
        const color = 'black'

        this._drawLine(0, this._y, this._width, this._y, 2, color)
        this._drawLine(0, this._y + this._space, this._width, this._y + this._space, 2, color)
    }

    _drawLine(startX, startY, endX, endY, width, color) {
        this._context.strokeStyle = color
        this._context.lineWidth = width

        this._context.beginPath()
        this._context.moveTo(startX, startY)
        this._context.lineTo(endX, endY)
        this._context.closePath()
        this._context.stroke()
    }
}
