class Letters {
    constructor(app) {
        this.APP = app

        this.NAME = {
            X: 100,
            Y: 200,
            LetterWidth: 200,
            LetterHeight: 400,
            LetterSpace: 100,
            RectWidth: 50,
        }

        this.JUMP_DURATION = 1000

        this.JUMP = {
            Height: 150,
            Duration: this.JUMP_DURATION,
            Gravity: 9.8,
            Phases: [0, this.JUMP_DURATION / 4, this.JUMP_DURATION / 2],
        }

        this._context = this.initCanvas()
        this._startTime = performance.now()
    }

    // TODO нейминг
    app(now) {
        this._context.clearRect(0, 0, this.APP.Width, this.APP.Height);
        this.drawName(now - this._startTime);
        requestAnimationFrame((now) => this.app(now));
    }

    drawName(time) {
        this._context.fillStyle = this.APP.Color
        this._context.fillRect(0, 0, this.APP.Width, this.APP.Height)

        this.drawR(0, time);
        this.drawD(1, time, -20);
        this.drawO(2, time);
    }

    drawR(index, time) {
        const jumpOffset = this.getJumpOffset(time, this.JUMP.Phases[index]);

        const R = {
            Color: "black",
            X: this.getLetterX(index),
            Y: this.NAME.Y + jumpOffset,
            Width: this.NAME.LetterWidth,
            Height: this.NAME.LetterHeight,
        }

        this._context.fillStyle = R.Color
        this._context.fillRect(R.X, R.Y, this.NAME.RectWidth, R.Height)

        this._context.lineWidth = this.NAME.RectWidth
        this._context.strokeStyle = R.Color
        this._context.beginPath()
        this._context.ellipse(
            R.X + R.Width / 2 + this.NAME.RectWidth / 2,
            R.Y + R.Height / 4 + this.NAME.RectWidth / 2,
            R.Width / 2,
            R.Height / 4,
            0, 0,
            2 * Math.PI)
        this._context.stroke()
    }

    drawD(index, time, offset = 0) {
        const jumpOffset = this.getJumpOffset(time, this.JUMP.Phases[index]);

        const D = {
            Color: "yellow",
            X: this.getLetterX(index) + offset,
            Y: this.NAME.Y + jumpOffset,
            Width: this.NAME.LetterWidth,
            Height: this.NAME.LetterHeight,
        }

        this._context.fillStyle = D.Color
        this._context.fillRect(D.X + D.Width - this.NAME.RectWidth, D.Y, this.NAME.RectWidth, D.Height)

        this._context.fillRect(D.X - 20, D.Y + D.Height - this.NAME.RectWidth, D.Width + 40, this.NAME.RectWidth)
        this.drawRotatedRect(D.X + D.Width / 3, D.Y, this.NAME.RectWidth, D.Height, 60, D.Color)
    }

    drawO(index, time) {
        const jumpOffset = this.getJumpOffset(time, this.JUMP.Phases[index]);
        const O = {
            Color: "white",
            X: this.getLetterX(index),
            Y: this.NAME.Y + this.NAME.RectWidth / 2 + jumpOffset,
            Width: this.NAME.LetterWidth,
            Height: this.NAME.LetterHeight - this.NAME.RectWidth,
        }

        this._context.fillStyle = O.Color
        this._context.beginPath()
        this._context.lineWidth = this.NAME.RectWidth
        this._context.strokeStyle = O.Color
        this._context.ellipse(
            O.X + O.Width / 2,
            O.Y + O.Height / 2,
            O.Width / 2,
            O.Height / 2,
            0, 0,
            2 * Math.PI,
        )
        this._context.stroke()
    }

    getLetterX(index) {
        return this.NAME.X + this.NAME.LetterWidth * index + this.NAME.LetterSpace * index
    }

    drawRotatedRect(x, y, width, height, angle, color) {
        this._context.save()

        this._context.translate(x + width / 2, y + height / 2)
        this._context.rotate(angle)
        this._context.fillStyle = color
        this._context.fillRect(-width / 2, -height / 2, width, height)

        this._context.restore()
    }

    getJumpOffset(baseTime, phaseOffset) {
        const currentTime = baseTime + phaseOffset;
        const cycleTime = currentTime % this.JUMP.Duration;

        const jumpTime = this.JUMP.Duration * 0.5;

        if (cycleTime >= jumpTime) {
            return 0;
        }

        const t = cycleTime / 1000;
        const totalJumpTimeInSeconds = jumpTime / 1000;

        const timeToPeak = totalJumpTimeInSeconds / 2;
        const g = (2 * this.JUMP.Height) / (timeToPeak * timeToPeak);

        const v0 = g * timeToPeak;

        if (t <= timeToPeak) {
            const height = v0 * t - 0.5 * g * t * t;
            return -height;
        }
        const fallTime = t - timeToPeak;
        const height = this.JUMP.Height - 0.5 * g * fallTime * fallTime;
        return height > 0
            ? -height
            : 0;
    }

    // TODO вынести наружу (передавать)
    initCanvas() {
        let canvas = document.createElement("canvas")
        canvas.width = this.APP.Width
        canvas.height = this.APP.Height

        document.body.appendChild(canvas)
        return canvas.getContext("2d")
    }
}
