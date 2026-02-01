class Lines {
    constructor(y, space, width) {
        this._y = y
        this._width = width
        this._space = space
    }

    getY()
    {
        return this._y
    }

    draw(context) {
        const color = 'black'

        this._drawLine(context, 0, this._y, this._width, this._y, 2, color)
        this._drawLine(context, 0, this._y + this._space, this._width, this._y + this._space, 2, color)
    }

    _drawLine(context, startX, startY, endX, endY, width, color) {
        context.strokeStyle = color
        context.lineWidth = width

        context.beginPath()
        context.moveTo(startX, startY)
        context.lineTo(endX, endY)
        context.closePath()
        context.stroke()
    }
}
