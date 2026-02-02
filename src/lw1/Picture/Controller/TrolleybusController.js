class TrolleybusController {
    constructor(model, view) {
        this._model = model;
        this._view = view;
        this._frameCount = 0;
        this._WHEEL_ROTATION_INTERVAL = 10;
        this._lastMousePos = null;
        this._lines = [];

        this._init();
    }

    update() {
        this._frameCount++;
        this._updateWheelRotation();
        this._model.move(!this._model.isDragging);
    }

    render() {
        this._context.clearRect(0, 0, APP.Width, APP.Height);
        this._lines.forEach(line => {
            line.draw(this._context)
        });
        this._view.draw(this._context);
    }

    async start() {
        await this._view.uploadPassengersImages(images);
        await this._view.uploadAdvertImage('./static/image/A_Serova.jpg');
        this._gameLoop();
    }

    _init() {
        this._initCanvas();
        this._initEventListeners();
        this._initLines();
    }

    _initCanvas() {
        let canvas = document.createElement("canvas");
        canvas.width = APP.Width;
        canvas.height = APP.Height;
        document.body.appendChild(canvas);
        this._context = canvas.getContext("2d");
    }

    _initLines() {
        const upperLines = 10;
        const bottomLines = 500;
        const spacing = 25;

        this._lines.push(new Lines(upperLines, spacing, APP.Width));
        this._lines.push(new Lines(bottomLines, spacing, APP.Width));
    }

    _initEventListeners() {
        window.addEventListener('mousedown', (event) => this._handleMouseDown(event));
        window.addEventListener('mouseup', () => this._handleMouseUp());
        window.addEventListener('mousemove', (event) => this._handleMouseMove(event));
    }

    _handleMouseDown(event) {
        if (!this._model.isPointInside(event.clientX, event.clientY)) {
            return;
        }

        this._model.isDragging = true;
    }

    _handleMouseUp() {
        this._lastMousePos = null;
        this._model.isDragging = false;

        const upperLinesY = 10;
        const bottomLinesY = 500;

        const hornsTopY = this._model.getHornsTopY(25);

        if (Math.abs(hornsTopY - upperLinesY) < Math.abs(hornsTopY - bottomLinesY)) {
            this._model.y = upperLinesY + 150;
            this._model.currentLine = Line.Top;
        } else {
            this._model.y = bottomLinesY + 150;
            this._model.currentLine = Line.Bottom;
        }
    }

    _handleMouseMove(event) {
        if (!this._model.isDragging) return;

        if (this._lastMousePos === null) {
            this._lastMousePos = {x: event.clientX, y: event.clientY};
        }

        const offsetX = event.clientX - this._lastMousePos.x;
        const offsetY = event.clientY - this._lastMousePos.y;

        this._model.updatePosition(offsetX, offsetY);
        this._lastMousePos = {
            x: event.clientX,
            y: event.clientY,
        };
    }

    _updateWheelRotation() {
        if (this._frameCount % this._WHEEL_ROTATION_INTERVAL === 0) {
            if (this._model.direction === Direction.Right) {
                this._model.wheelRotation += Math.PI / 2;
            } else {
                this._model.wheelRotation -= Math.PI / 2;
            }
        }

        if (this._frameCount >= 1000) {
            this._frameCount = 0;
        }
    }

    _gameLoop() {
        this.update();
        this.render();
        requestAnimationFrame(() => this._gameLoop());
    }
}
