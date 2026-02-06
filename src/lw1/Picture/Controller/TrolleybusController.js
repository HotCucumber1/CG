class TrolleybusController {
    constructor(model, view, lines) {
        this._model = model;
        this._view = view;
        this._frameCount = 0;
        this._WHEEL_ROTATION_INTERVAL = 10;
        this._lastMousePos = null;
        this._lines = lines;
        this._isDragging = false;

        this._initEventListeners();
    }

    update() {
        this._frameCount++;
        this._updateWheelRotation();
        if (!this._isDragging) {
            this._model.move();
        }
    }

    render() {
        this._view.draw();
        this._lines.forEach(line => line.draw());
    }

    async start() {
        await this._view.uploadPassengersImages(images);
        await this._view.uploadAdvertImage('./static/image/A_Serova.jpg');
        this._mainLoop();
    }

    // TODO Вынести во View работу с DOM
    _initEventListeners() {
        this._view.onMouseDown((event) => this._handleMouseDown(event));
        this._view.onMouseUp(() => this._handleMouseUp());
        this._view.onMouseMove((event) => this._handleMouseMove(event))
    }

    _handleMouseDown(event) {
        if (!this._isPointInsideTrolleybus(event.clientX, event.clientY)) {
            return;
        }

        this._isDragging = true;
    }

    // TODO вынести в контроллер из модели
    _isPointInsideTrolleybus(x, y) {
        return x >= this._model.x &&
            x <= this._model.x + this._model.width &&
            y >= this._model.y &&
            y <= this._model.y + this._model.height;
    }

    _handleMouseUp() {
        this._lastMousePos = null;
        this._isDragging = false;

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
        if (!this._isDragging) {
            return;
        }

        if (this._lastMousePos === null) {
            this._lastMousePos = {
                x: event.clientX,
                y: event.clientY,
            };
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
        if (this._isDragging) {
            return
        }
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

    _mainLoop() {
        this.update();
        this.render();
        requestAnimationFrame(() => this._mainLoop());
    }
}
