class Model {
    constructor() {
        this._balls = [];
        this._maxBalls = 50;
        this._gravity = 0.3;
        this._energyLoss = 0.8;
        this._cannon = {
            x: 400,
            y: 300,
            angle: 0
        };
        this._obstacles = this._createObstacles();
        this._canvasHeight = 600;
        this._bounds = {
            left: 0,
            right: 800,
            top: 0,
            bottom: 600
        };
        this._substeps = 5;
    }

    get obstacles() {
        return this._obstacles
    }

    get cannon() {
        return this._cannon
    }

    get balls() {
        return this._balls
    }

    addBall(x, y, vx, vy) {
        if (this._balls.length < this._maxBalls) {
            this._balls.push(new Ball(x, y, vx, vy));
            return true;
        }
        return false;
    }

    update() {
        const gravityStep = this._gravity / this._substeps;

        for (let step = 0; step < this._substeps; step++) {
            for (let ball of this.balls) {
                ball.update(gravityStep);
            }
            this._handleCollisions();
        }

        for (let i = 0; i < this.balls.length; i++) {
            const ball = this.balls[i];
            if (ball.y > this._canvasHeight + ball.radius) {
                this.balls.splice(i, 1);
            }
        }
    }

    setCannonAngle(angle) {
        this._cannon.angle = angle;
    }

    shootAt(targetX, targetY) {
        const speed = 3;

        const dx = targetX - this._cannon.x;
        const dy = targetY - this._cannon.y;
        const distance = Math.sqrt(dx * dx + dy * dy);

        const vx = (dx / distance) * speed;
        const vy = (dy / distance) * speed;
        return this.addBall(this._cannon.x, this._cannon.y, vx, vy);
    }

    _handleCollisions() {
        for (let ball of this.balls) {
            this._handleWallCollisions(ball);
        }

        for (let ball of this.balls) {
            for (let obstacle of this.obstacles) {
                obstacle.resolveCollision(ball, this._energyLoss);
            }
        }
    }

    _handleWallCollisions(ball) {
        if (ball.x - ball.radius < this._bounds.left) {
            ball.x = this._bounds.left + ball.radius;
            ball.vx = -ball.vx * this._energyLoss;
        }
        if (ball.x + ball.radius > this._bounds.right) {
            ball.x = this._bounds.right - ball.radius;
            ball.vx = -ball.vx * this._energyLoss;
        }
        if (ball.y - ball.radius < this._bounds.top) {
            ball.y = this._bounds.top + ball.radius;
            ball.vy = -ball.vy * this._energyLoss;
        }
        if (ball.y + ball.radius > this._bounds.bottom) {
            this._removeBall(ball)
        }
    }

    _createObstacles() {
        return [
            new Obstacle(250, 250, 150, 20, Math.PI / 6),
            new Obstacle(500, 450, 200, 20, -Math.PI / 4),
            new Obstacle(300, 500, 120, 20, Math.PI / 3),
            new Obstacle(600, 200, 180, 20, Math.PI / 2),
        ];
    }

    _removeBall(ball) {
        const index = this._balls.indexOf(ball);
        if (index > -1) {
            this._balls.splice(index, 1);
        }
    }
}
