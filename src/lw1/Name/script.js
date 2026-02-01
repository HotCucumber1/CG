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

function drawName(context) {
    context.fillStyle = APP.Color
    context.fillRect(0, 0, APP.Width, APP.Height)

    drawR(context, 0)
    drawD(context, 1, -20)
    drawO(context, 2)
}

function drawR(context, index) {
    const R = {
        Color: "black",
        X: getLetterX(index),
        Y: NAME.Y,
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

function drawD(context, index, offset = 0) {
    const D = {
        Color: "yellow",
        X: getLetterX(index) + offset,
        Y: NAME.Y,
        Width: NAME.LetterWidth,
        Height: NAME.LetterHeight,
    }

    context.fillStyle = D.Color
    context.fillRect(D.X + D.Width - NAME.RectWidth, D.Y, NAME.RectWidth, D.Height)

    context.fillRect(D.X - 20, D.Y + D.Height - NAME.RectWidth, D.Width + 40, NAME.RectWidth)
    drawRotatedRect(context, D.X + D.Width / 3, D.Y, NAME.RectWidth, D.Height, 60, D.Color)
}

function drawO(context, index) {
    const O = {
        Color: "white",
        X: getLetterX(index),
        Y: NAME.Y + NAME.RectWidth / 2,
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

function initCanvas() {
    let canvas = document.createElement("canvas")
    canvas.width = APP.Width
    canvas.height = APP.Height

    document.body.appendChild(canvas)
    return canvas.getContext("2d")
}


let context = initCanvas()
drawName(context)
