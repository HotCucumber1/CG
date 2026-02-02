// const Line = Object.freeze({
//     Top: 'top',
//     Bottom: 'bottom',
// });
//
// function initCanvas() {
//     let canvas = document.createElement("canvas")
//     canvas.width = APP.Width
//     canvas.height = APP.Height
//
//     document.body.appendChild(canvas)
//     return canvas.getContext("2d")
// }
//
// let TModel = {
//     xSpeed: -5
// }
//
// function moveTrolleybus(trolleybus, needUpdate) {
//     if (!needUpdate) {
//         return
//     }
//
//     frameCount++
//     trolleybus.setX(trolleybus.getX() + TModel.xSpeed)
//     if (frameCount % WHEEL_ROTATION_INTERVAL === 0) {
//         trolleybus.addWheelRotation(Math.PI / 2);
//     }
//     if (frameCount >= 1000) {
//         frameCount = 0;
//     }
//
//     if (trolleybus.getX() + trolleybus.getWidth() < 0 || trolleybus.getX() > APP.Width) {
//         TModel.xSpeed *= (-1)
//         if (trolleybus.getDirection() === Direction.Right) {
//             trolleybus.setDirection(Direction.Left)
//         } else {
//             trolleybus.setDirection(Direction.Right)
//         }
//
//         if (currentLine === Line.Top) {
//             trolleybus.setY(trolleybus.getY() + distanceBetweenLines)
//             currentLine = Line.Bottom
//         } else {
//             trolleybus.setY(trolleybus.getY() - distanceBetweenLines)
//             currentLine = Line.Top
//         }
//     }
// }
//
// function updateTrolleybusPos(trolleybus, xOffset, yOffset) {
//     trolleybus.setX(trolleybus.getX() + xOffset)
//     trolleybus.setY(trolleybus.getY() + yOffset)
// }
//
// function app() {
//     context.clearRect(0, 0, APP.Width, APP.Height)
//     lines1.draw(context)
//     lines2.draw(context)
//     trolleybus.draw(context)
//     moveTrolleybus(trolleybus, !isDragAndDrop)
//
//     requestAnimationFrame(app)
// }
//
// function initEventsListener() {
//     window.addEventListener('mousedown', (event) => {
//         if (event.clientX < trolleybus.getX() ||
//             event.clientX > trolleybus.getX() + trolleybus.getWidth() ||
//             event.clientY < trolleybus.getY() ||
//             event.clientY > trolleybus.getY() + trolleybus.getHeight()) {
//             return;
//         }
//
//         isDragAndDrop = true
//     })
//     window.addEventListener('mouseup', () => {
//         lastPos = null
//         isDragAndDrop = false
//
//         if (Math.abs(trolleybus.getHornsTopY() - lines1.getY()) <
//             Math.abs(trolleybus.getHornsTopY() - lines2.getY())) {
//             trolleybus.setY(upperLines + 150)
//             currentLine = Line.Top
//         } else {
//             trolleybus.setY(bottomLines + 150)
//             currentLine = Line.Bottom
//         }
//     })
//
//     window.addEventListener('mousemove', (event) => {
//         if (!isDragAndDrop) {
//             return
//         }
//
//         if (lastPos === null) {
//             lastPos = {
//                 x: event.clientX,
//                 y: event.clientY,
//             }
//         }
//
//         let offsetX = event.clientX - lastPos.x
//         let offsetY = event.clientY - lastPos.y
//
//         updateTrolleybusPos(trolleybus, offsetX, offsetY)
//         lastPos = {
//             x: event.clientX,
//             y: event.clientY,
//         }
//     })
// }
//
//
// let isDragAndDrop = false
// let lastPos = null
//
//
// let frameCount = 0
// const WHEEL_ROTATION_INTERVAL = 10
//
// const distanceBetweenLines = 490
// const upperLines = 10
// const bottomLines = 500
//
// let currentLine = Line.Top
//
// let context = initCanvas()
// let trolleybus = new TrolleybusView(
//     APP.Width,
//     160,
//     '#FF603D',
//     images.length,
//     25,
//     Direction.Left,
// )
//
// let lines1 = new Lines(upperLines, 25, APP.Width)
// let lines2 = new Lines(bottomLines, 25, APP.Width)
//
// initEventsListener()
// trolleybus
//     .uploadPassengersImages(images)
//     .then(() =>
//         trolleybus.uploadAdvertImage('./static/image/A_Serova.jpg').then(() => {
//             app()
//         })
//     )


const APP = {
    Width: window.innerWidth,
    Height: window.innerHeight
};

document.addEventListener('DOMContentLoaded', () => {
    const model = new TrolleybusModel(APP.Width, 160, Direction.Left);
    const view = new TrolleybusView(model, '#FF603D', images.length, 25);
    const controller = new TrolleybusController(model, view);

    controller.start();
});
