class View {
    constructor(canvasId, model) {
        /**
         * @type {HTMLCanvasElement}
         * @private
         */
        this._canvas = document.getElementById(canvasId);
        this._canvasContext = this._canvas.getContext('2d');
        this._ballCountElement = document.getElementById('ballCount');
        this._model = model
    }

    render() {
        this._clear();
        this._drawObstacles();
        this._drawBalls();
        this._drawCannon();
        this._updateBallCount(this._model.balls.length);
    }

    onMouseMove(callback) {
        this._canvas.addEventListener('mousemove', (e) => callback(e.clientX, e.clientY));
    }

    onMouseClick(callback) {
        this._canvas.addEventListener('click', (e) => callback(e.clientX, e.clientY));
    }

    _clear() {
        const backgroundColor = '#1a1a2e'
        const borderColor = '#4a4a6a'

        this._canvasContext.clearRect(0, 0, this._canvas.width, this._canvas.height);

        this._canvasContext.fillStyle = backgroundColor;
        this._canvasContext.fillRect(0, 0, this._canvas.width, this._canvas.height);

        this._canvasContext.strokeStyle = borderColor;
        this._canvasContext.lineWidth = 2;
        this._canvasContext.strokeRect(0, 0, this._canvas.width, this._canvas.height);
    }

    _drawObstacles() {
        const fillColor = '#4a90e2';
        const strokeColor = '#fff';

        this._canvasContext.save();
        this._model.obstacles.forEach(obstacle => {
            this._canvasContext.translate(obstacle._x, obstacle._y);
            this._canvasContext.rotate(obstacle.angle);

            this._canvasContext.fillStyle = fillColor;
            this._canvasContext.shadowColor = 'rgba(0,0,0,0.3)';
            this._canvasContext.shadowBlur = 10;
            this._canvasContext.fillRect(-obstacle.width / 2, -obstacle.height / 2, obstacle.width, obstacle.height);

            this._canvasContext.strokeStyle = strokeColor;
            this._canvasContext.lineWidth = 2;
            this._canvasContext.shadowBlur = 0;
            this._canvasContext.strokeRect(-obstacle.width / 2, -obstacle.height / 2, obstacle.width, obstacle.height);

            this._canvasContext.resetTransform();
        });
        this._canvasContext.restore();
    }

    _drawCannon() {
        const length = 40;
        const width = 15;
        const fillColor = '#e74c3c';
        const strokeColor = '#c0392b';

        this._canvasContext.save();
        this._canvasContext.translate(this._model.cannon.x, this._model.cannon.y);
        this._canvasContext.rotate(this._model.cannon.angle);

        this._canvasContext.fillStyle = fillColor;
        this._canvasContext.shadowColor = 'rgba(0,0,0,0.3)';
        this._canvasContext.shadowBlur = 10;
        this._canvasContext.beginPath();
        this._canvasContext.rect(0, -width / 2, length, width);
        this._canvasContext.fill();

        this._canvasContext.fillStyle = strokeColor;
        this._canvasContext.beginPath();
        this._canvasContext.arc(0, 0, width, 0, Math.PI * 2);
        this._canvasContext.fill();

        this._canvasContext.restore();
    }

    _drawBalls() {
        this._model.balls.forEach(ball => {
            this._canvasContext.save();
            this._canvasContext.translate(ball.x, ball.y);

            this._canvasContext.fillStyle = ball.color;
            this._canvasContext.shadowColor = 'rgba(0,0,0,0.3)';
            this._canvasContext.shadowBlur = 10;
            this._canvasContext.beginPath();
            this._canvasContext.arc(0, 0, ball.radius, 0, Math.PI * 2);
            this._canvasContext.fill();

            this._canvasContext.fillStyle = 'rgba(255,255,255,0.3)';
            this._canvasContext.beginPath();
            this._canvasContext.arc(-2, -2, 3, 0, Math.PI * 2);
            this._canvasContext.fill();

            this._canvasContext.restore();
        });
    }

    _updateBallCount(count) {
        this._ballCountElement.textContent = count;
    }
}
