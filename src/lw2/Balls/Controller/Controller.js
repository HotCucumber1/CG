class Controller {
    constructor(model, view, soundManager) {
        this.model = model;
        this.view = view;
        this._soundManager = soundManager
        this._initEventListeners();
    }

    _initEventListeners() {
        this.view.onMouseMove((x, y) => this._rotateCannon(x, y))
        this.view.onMouseClick((x, y) => this._makeShoot(x, y))
    }

    _rotateCannon(x, y) {
        const rect = this.view._canvas.getBoundingClientRect();
        const mouseX = x - rect.left;
        const mouseY = y - rect.top;

        const dx = mouseX - this.model.cannon.x;
        const dy = mouseY - this.model.cannon.y;
        const angle = Math.atan2(dy, dx);

        this.model.setCannonAngle(angle);
    }

    _makeShoot(x, y) {
        const rect = this.view._canvas.getBoundingClientRect();
        const mouseX = x - rect.left;
        const mouseY = y - rect.top;

        const success = this.model.shootAt(mouseX, mouseY);
        if (success) {
            this._soundManager.play()
        }
    }
}
