const Direction = Object.freeze({
    Right: 'right',
    Left: 'left',
});

const Line = Object.freeze({
    Top: 'top',
    Bottom: 'bottom',
});

class TrolleybusModel {
    constructor(x, y, direction = Direction.Left) {
        this._x = x;
        this._y = y;
        this._direction = direction;
        this._xSpeed = -5;
        this._wheelRotation = Math.PI / 2;
        this._currentLine = Line.Top;

        this._width = 1000;
        this._height = 200;
        this._engineWidth = this._width / 4;
        this._engineHeight = this._height / 8;
    }

    get x() {
        return this._x;
    }

    set x(value) {
        this._x = value;
    }

    get y() {
        return this._y;
    }

    set y(value) {
        this._y = value;
    }

    get direction() {
        return this._direction;
    }

    set direction(value) {
        this._direction = value;
    }

    get wheelRotation() {
        return this._wheelRotation;
    }

    set wheelRotation(value) {
        this._wheelRotation = value;
    }

    get currentLine() {
        return this._currentLine;
    }

    set currentLine(value) {
        this._currentLine = value;
    }

    get width() {
        return this._width;
    }

    get height() {
        return this._height;
    }

    get engineWidth() {
        return this._engineWidth;
    }

    get engineHeight() {
        return this._engineHeight;
    }

    move() {
        this._x += this._xSpeed;

        if (this._x + this._width < 0 || this._x > APP.Width) {
            this._xSpeed *= -1;
            this._direction = this._direction === Direction.Right ? Direction.Left : Direction.Right;
            this._switchLine();
        }
    }

    updatePosition(xOffset, yOffset) {
        this._x += xOffset;
        this._y += yOffset;
    }

    get engineX() {
        return this._x + this._width / 2 - this._engineWidth / 2;
    }

    get engineY() {
        return this._y - this._engineHeight;
    }

    getHornsTopY(hornsSpace) {
        return this.engineY - this._height / 2 - hornsSpace;
    }

    _switchLine() {
        const distanceBetweenLines = 490;

        if (this._currentLine === Line.Top) {
            this._y += distanceBetweenLines;
            this._currentLine = Line.Bottom;
        } else {
            this._y -= distanceBetweenLines;
            this._currentLine = Line.Top;
        }
    }
}
