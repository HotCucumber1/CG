const APP = {
    Width: 1200,
    Height: 900,
    Color: '#00bfff',
}

const NAME = {
    X: 100,
    Y: 200,
    LetterWidth: 200,
    LetterHeight: 400,
    LetterSpace: 100,
    RectWidth: 50,
}

const JUMP_DURATION = 1000

const JUMP = {
    Height: 150,
    Duration: JUMP_DURATION,
    Gravity: 9.8,
    Phases: [0, JUMP_DURATION / 4, JUMP_DURATION / 2],
}

function drawName(context, time) {
    context.fillStyle = APP.Color
    context.fillRect(0, 0, APP.Width, APP.Height)

    drawR(context, 0, time);
    drawD(context, 1, time, -20);
    drawO(context, 2, time);
}

function drawR(context, index, time) {
    const jumpOffset = calculateJumpOffset(time, JUMP.Phases[index]);

    const R = {
        Color: "black",
        X: getLetterX(index),
        Y: NAME.Y + jumpOffset,
        Width: NAME.LetterWidth,
        Height: NAME.LetterHeight,
    }

    context.fillStyle = R.Color
    context.fillRect(R.X, R.Y, NAME.RectWidth, R.Height)

    context.lineWidth = NAME.RectWidth
    context.strokeStyle = R.Color
    context.beginPath()
    context.ellipse(
        R.X + R.Width / 2 + NAME.RectWidth / 2,
        R.Y + R.Height / 4 + NAME.RectWidth / 2,
        R.Width / 2,
        R.Height / 4,
        0, 0,
        2 * Math.PI)
    context.stroke()
}

function drawD(context, index, time, offset = 0) {
    const jumpOffset = calculateJumpOffset(time, JUMP.Phases[index]);

    const D = {
        Color: "yellow",
        X: getLetterX(index) + offset,
        Y: NAME.Y + jumpOffset,
        Width: NAME.LetterWidth,
        Height: NAME.LetterHeight,
    }

    context.fillStyle = D.Color
    context.fillRect(D.X + D.Width - NAME.RectWidth, D.Y, NAME.RectWidth, D.Height)

    context.fillRect(D.X - 20, D.Y + D.Height - NAME.RectWidth, D.Width + 40, NAME.RectWidth)
    drawRotatedRect(context, D.X + D.Width / 3, D.Y, NAME.RectWidth, D.Height, 60, D.Color)
}

function drawO(context, index, time) {
    const jumpOffset = calculateJumpOffset(time, JUMP.Phases[index]);
    const O = {
        Color: "white",
        X: getLetterX(index),
        Y: NAME.Y + NAME.RectWidth / 2 + jumpOffset,
        Width: NAME.LetterWidth,
        Height: NAME.LetterHeight - NAME.RectWidth,
    }

    context.fillStyle = O.Color
    context.beginPath()
    context.lineWidth = NAME.RectWidth
    context.strokeStyle = O.Color
    context.ellipse(
        O.X + O.Width / 2,
        O.Y + O.Height / 2,
        O.Width / 2,
        O.Height / 2,
        0, 0,
        2 * Math.PI,
    )
    context.stroke()
}

function getLetterX(index) {
    return NAME.X + NAME.LetterWidth * index + NAME.LetterSpace * index
}

function drawRotatedRect(context, x, y, width, height, angle, color) {
    context.save()

    context.translate(x + width / 2, y + height / 2)
    context.rotate(angle)
    context.fillStyle = color
    context.fillRect(-width / 2, -height / 2, width, height)

    context.restore()
}

function calculateJumpOffset(baseTime, phaseOffset) {
    const currentTime = baseTime + phaseOffset;
    const cycleTime = currentTime % JUMP.Duration;

    const jumpTime = JUMP.Duration * 0.5;

    if (cycleTime >= jumpTime) {
        return 0;
    }

    const t = cycleTime / 1000;
    const totalJumpTimeInSeconds = jumpTime / 1000;

    const timeToPeak = totalJumpTimeInSeconds / 2;
    const g = (2 * JUMP.Height) / (timeToPeak * timeToPeak);

    const v0 = g * timeToPeak;

    if (t <= timeToPeak) {
        const height = v0 * t - 0.5 * g * t * t;
        return -height;
    }
    const fallTime = t - timeToPeak;
    const height = JUMP.Height - 0.5 * g * fallTime * fallTime;
    return height > 0
        ? -height
        : 0;
}

function initCanvas() {
    let canvas = document.createElement("canvas")
    canvas.width = APP.Width
    canvas.height = APP.Height

    document.body.appendChild(canvas)
    return canvas.getContext("2d")
}

function app(now) {
    context.clearRect(0, 0, APP.Width, APP.Height);
    drawName(context, now - startTime);
    requestAnimationFrame(app);
}


let context = initCanvas()
let startTime = performance.now()
app(startTime)
