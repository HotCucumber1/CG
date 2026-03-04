class App {
    constructor() {
        this._model = new Model();
        this._view = new View('gameCanvas', this._model);
        this._sound = new SoundManager('./static/sounds/shot-sound.mp3')
        new Controller(
            this._model,
            this._view,
            this._sound,
        );
    }

    start() {
        requestAnimationFrame(() => this._animate());
    }

    _animate() {
        this._model.update();
        this._view.render();
        requestAnimationFrame(() => this._animate());
    }
}

document.addEventListener('DOMContentLoaded', () => {
    const app = new App();
    app.start();
});
