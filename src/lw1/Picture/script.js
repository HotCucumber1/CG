const APP = {
    Width: window.innerWidth,
    Height: window.innerHeight
};

document.addEventListener('DOMContentLoaded', () => {
    const upperLines = 10;
    const bottomLines = 500;
    const spacing = 25;

    const canvas = document.createElement("canvas");
    canvas.width = APP.Width;
    canvas.height = APP.Height;
    document.body.appendChild(canvas);
    const context = canvas.getContext("2d")

    const model = new TrolleybusModel(APP.Width, 160, Direction.Left);
    const view = new TrolleybusView(model, '#FF603D', images.length, spacing, context);

    const controller = new TrolleybusController(model, view, [
        new Lines(upperLines, spacing, APP.Width, context),
        new Lines(bottomLines, spacing, APP.Width, context),
    ]);

    controller.start();
});
