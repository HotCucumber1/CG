class Ball {
    constructor(x, y, vx, vy) {
        this._x = x;
        this._y = y;
        this._vx = vx;
        this._vy = vy;
        this._radius = 8;
        this._color = this._getRandomColor();
    }

    update(gravity) {
        this._vy += gravity;
        this._x += this._vx;
        this._y += this._vy;
    }

    get x() {
        return this._x
    }

    set x(x) {
        this._x = x
    }

    get y() {
        return this._y
    }

    set y(y) {
        this._y = y
    }

    get radius() {
        return this._radius
    }

    get color() {
        return this._color
    }

    _getRandomColor() {
        const hue = Math.random() * 360;
        return `hsl(${hue}, 70%, 60%)`;
    }
}
