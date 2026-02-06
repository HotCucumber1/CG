class TrolleybusView {
    constructor(model, color, windowsCount, hornsSpace, context) {
        this._model = model;
        this._color = color;
        this._windowsCount = windowsCount;
        this._hornsSpace = hornsSpace;

        const spaceForWindows = this._model.width * 0.9
        const spaceForWindowSpacing = this._model.width * 0.1

        this._windowHeight = this._model.height / 2;
        this._windowWidth = spaceForWindows / this._windowsCount;
        this._windowSpacing = spaceForWindowSpacing / this._windowsCount;

        this._context = context;
        this._imagePaths = [];
        this._advertImgSrc = null;
        this._loader = new ImageLoader();
    }

    async uploadPassengersImages(images) {
        this._imagePaths = images;
        await this._loader.loadAllImages(this._imagePaths);
    }

    async uploadAdvertImage(image) {
        this._advertImgSrc = image;
        await this._loader.loadImage(image);
    }

    draw() {
        this._context.clearRect(0, 0, APP.Width, APP.Height);
        this._context.save();

        if (this._model.direction === Direction.Right) {
            this._context.translate(this._model.x + this._model.width, 0);
            this._context.scale(-1, 1);
            this._context.translate(-this._model.x, 0);
        }

        this._drawBody();
        const windowStartPoint = this._drawWindows();
        this._drawWheels();
        this._drawEngine();
        this._drawHorns();
        this._drawPassengers(windowStartPoint);
        this._drawAdvert();

        this._context.restore();
    }

    onMouseDown(callback) {
        window.addEventListener('mousedown', callback);
        document.body.style.cursor = 'grabbing';
    }

    onMouseUp(callback) {
        window.addEventListener('mouseup', callback);
        document.body.style.cursor = 'default';
    }

    onMouseMove(callback) {
        window.addEventListener('mousemove', callback);
    }

    _drawBody() {
        const roundRadius = 10
        this._context.fillStyle = this._color;
        this._context.roundRect(
            this._model.x,
            this._model.y,
            this._model.width,
            this._model.height,
            roundRadius,
        );
        this._context.fill();
    }

    _drawWindows() {
        this._context.fillStyle = '#00bfff';
        const startX = this._model.x + this._windowSpacing / 2;
        const startY = this._model.y + this._windowSpacing / 2;

        for (let i = 0; i < this._windowsCount; i++) {
            this._context.fillRect(
                startX + this._windowWidth * i + this._windowSpacing * i,
                startY,
                this._windowWidth,
                this._windowHeight,
            );
        }

        return {x: startX, y: startY};
    }

    _drawWheels() {
        const frontWheelX = this._model.x + this._model.width * 0.1;
        const backWheelX = this._model.x + this._model.width * 0.8;

        this._drawWheel(frontWheelX, this._model.y + this._model.height);
        this._drawWheel(backWheelX, this._model.y + this._model.height);
    }

    _drawWheel(x, y) {
        const wheelColor = '#222222';
        const diskColor = '#656565';
        const wheelRadius = this._model.height / 4;

        this._drawEllipse(
            x, y,
            wheelRadius,
            wheelRadius * 0.9,
            this._model.wheelRotation,
            wheelColor,
        );
        this._drawEllipse(
            x, y,
            wheelRadius * 0.6,
            wheelRadius * 0.6 * 0.9,
            this._model.wheelRotation,
            diskColor,
        );
    }

    _drawEngine() {
        this._context.fillStyle = '#222222';
        this._context.fillRect(
            this._model.engineX,
            this._model.engineY,
            this._model.engineWidth,
            this._model.engineHeight,
        );
    }

    _drawHorns() {
        const hornColor = '#222222';

        const frontHornXStart = this._model.engineX + this._model.engineWidth / 2 - this._hornsSpace / 2;
        const backHornXStart = this._model.engineX + this._model.engineWidth / 2 + this._hornsSpace / 2;

        const frontHornEnd = {
            x: frontHornXStart + this._model.height / 2 + this._hornsSpace,
            y: this._model.engineY - this._model.height / 2 - this._hornsSpace,
        };

        const backHornEnd = {
            x: backHornXStart + this._model.height / 2,
            y: this._model.engineY - this._model.height / 2,
        };

        this._drawLine(
            frontHornXStart,
            this._model.engineY,
            frontHornEnd.x,
            frontHornEnd.y,
            2, hornColor,
        );
        this._drawCircle(frontHornEnd.x, frontHornEnd.y, 5, hornColor);

        this._drawLine(
            backHornXStart,
            this._model.engineY,
            backHornEnd.x,
            backHornEnd.y,
            2, hornColor,
        );
        this._drawCircle(backHornEnd.x, backHornEnd.y, 5, hornColor);
    }

    _drawPassengers(windowStartPoint) {
        const k = 0.8;
        this._imagePaths.forEach((imagePath, i) => {
            const image = this._loader.getImage(imagePath);

            let targetHeight, scale;
            if (i === 0) {
                targetHeight = this._windowHeight;
                scale = this._windowHeight / image.height;
            } else {
                targetHeight = this._windowHeight * k;
                scale = this._windowHeight * k / image.height;
            }

            const targetWidth = image.width * scale;
            const y = windowStartPoint.y + (this._windowHeight - targetHeight);
            const x = windowStartPoint.x + (this._windowWidth - targetWidth);

            this._context.drawImage(
                image,
                x + i * this._windowWidth + i * this._windowSpacing,
                y,
                targetWidth,
                targetHeight,
            );
        });
    }

    _drawAdvert() {
        if (this._advertImgSrc === null) {
            return;
        }

        const image = this._loader.getImage(this._advertImgSrc);
        const targetHeight = this._model.height - this._windowHeight - this._windowSpacing / 2;
        const scale = targetHeight / image.height;
        const targetWidth = image.width * scale;

        this._context.drawImage(
            image,
            this._model.x + this._model.width / 2 - targetWidth / 2,
            this._model.y + this._windowHeight + this._windowSpacing / 2,
            targetWidth,
            targetHeight
        );
    }

    _drawCircle(x, y, r, color) {
        this._context.fillStyle = color;
        this._context.beginPath();
        this._context.arc(x, y, r, 0, 2 * Math.PI);
        this._context.fill();
    }

    _drawEllipse(x, y, rx, ry, rotation, color) {
        this._context.fillStyle = color;
        this._context.beginPath();
        this._context.ellipse(x, y, rx, ry, rotation, 0, 2 * Math.PI);
        this._context.fill();
    }

    _drawLine(startX, startY, endX, endY, width, color) {
        this._context.strokeStyle = color;
        this._context.lineWidth = width;
        this._context.beginPath();
        this._context.moveTo(startX, startY);
        this._context.lineTo(endX, endY);
        this._context.closePath();
        this._context.stroke();
    }
}
