class TrolleybusView {
    constructor(model, color, windowsCount, hornsSpace) {
        this._model = model;
        this._color = color;
        this._windowsCount = windowsCount;
        this._hornsSpace = hornsSpace;

        this._r = 10;
        this._windowHeight = this._model.height / 2;
        this._windowWidth = this._model.width * 0.9 / this._windowsCount;
        this._windowSpacing = this._model.width * 0.1 / this._windowsCount;

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

    draw(context) {
        context.save();

        if (this._model.direction === Direction.Right) {
            context.translate(this._model.x + this._model.width, 0);
            context.scale(-1, 1);
            context.translate(-this._model.x, 0);
        }

        this._drawBody(context);
        const windowStartPoint = this._drawWindows(context);
        this._drawWheels(context);
        this._drawEngine(context);
        this._drawHorns(context);
        this._drawPassengers(context, windowStartPoint);
        this._drawAdvert(context);

        context.restore();
    }

    _drawBody(context) {
        context.fillStyle = this._color;
        context.roundRect(
            this._model.x,
            this._model.y,
            this._model.width,
            this._model.height,
            this._r,
        );
        context.fill();
    }

    _drawWindows(context) {
        context.fillStyle = '#00bfff';
        const startX = this._model.x + this._windowSpacing / 2;
        const startY = this._model.y + this._windowSpacing / 2;

        for (let i = 0; i < this._windowsCount; i++) {
            context.fillRect(
                startX + this._windowWidth * i + this._windowSpacing * i,
                startY,
                this._windowWidth,
                this._windowHeight,
            );
        }

        return {x: startX, y: startY};
    }

    _drawWheels(context) {
        const frontWheelX = this._model.x + this._model.width * 0.1;
        const backWheelX = this._model.x + this._model.width * 0.8;

        this._drawWheel(context, frontWheelX, this._model.y + this._model.height);
        this._drawWheel(context, backWheelX, this._model.y + this._model.height);
    }

    _drawWheel(context, x, y) {
        const wheelColor = '#222222';
        const diskColor = '#656565';
        const wheelRadius = this._model.height / 4;

        this._drawEllipse(
            context,
            x, y,
            wheelRadius,
            wheelRadius * 0.9,
            this._model.wheelRotation,
            wheelColor,
        );
        this._drawEllipse(
            context,
            x, y,
            wheelRadius * 0.6,
            wheelRadius * 0.6 * 0.9,
            this._model.wheelRotation,
            diskColor,
        );
    }

    _drawEngine(context) {
        context.fillStyle = '#222222';
        context.fillRect(
            this._model.engineX,
            this._model.engineY,
            this._model.engineWidth,
            this._model.engineHeight,
        );
    }

    _drawHorns(context) {
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
            context,
            frontHornXStart,
            this._model.engineY,
            frontHornEnd.x,
            frontHornEnd.y,
            2, hornColor,
        );
        this._drawCircle(context, frontHornEnd.x, frontHornEnd.y, 5, hornColor);

        this._drawLine(
            context,
            backHornXStart,
            this._model.engineY,
            backHornEnd.x,
            backHornEnd.y,
            2, hornColor,
        );
        this._drawCircle(context, backHornEnd.x, backHornEnd.y, 5, hornColor);
    }

    _drawPassengers(context, windowStartPoint) {
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

            context.drawImage(
                image,
                x + i * this._windowWidth + i * this._windowSpacing,
                y,
                targetWidth,
                targetHeight,
            );
        });
    }

    _drawAdvert(context) {
        if (this._advertImgSrc === null) return;

        const image = this._loader.getImage(this._advertImgSrc);
        const targetHeight = this._model.height - this._windowHeight - this._windowSpacing / 2;
        const scale = targetHeight / image.height;
        const targetWidth = image.width * scale;

        context.drawImage(
            image,
            this._model.x + this._model.width / 2 - targetWidth / 2,
            this._model.y + this._windowHeight + this._windowSpacing / 2,
            targetWidth,
            targetHeight
        );
    }

    _drawCircle(context, x, y, r, color) {
        context.fillStyle = color;
        context.beginPath();
        context.arc(x, y, r, 0, 2 * Math.PI);
        context.fill();
    }

    _drawEllipse(context, x, y, rx, ry, rotation, color) {
        context.fillStyle = color;
        context.beginPath();
        context.ellipse(x, y, rx, ry, rotation, 0, 2 * Math.PI);
        context.fill();
    }

    _drawLine(context, startX, startY, endX, endY, width, color) {
        context.strokeStyle = color;
        context.lineWidth = width;
        context.beginPath();
        context.moveTo(startX, startY);
        context.lineTo(endX, endY);
        context.closePath();
        context.stroke();
    }
}
