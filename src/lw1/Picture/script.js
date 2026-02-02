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
