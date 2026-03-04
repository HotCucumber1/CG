class Obstacle {
    constructor(x, y, width, height, angle) {
        this._x = x;
        this._y = y;
        this._width = width;
        this._height = height;
        this._angle = angle;
    }

    get x() {
        return this._x;
    }

    get y() {
        return this._y;
    }

    get width() {
        return this._width;
    }

    get height() {
        return this._height;
    }

    get angle() {
        return this._angle;
    }

    resolveCollision(ball, energyLoss) {
        const local = this._worldToLocal(ball.x, ball.y);
        const localX = local.x;
        const localY = local.y;

        const halfW = this._width / 2;
        const halfH = this._height / 2;

        const isOverlapX = Math.abs(localX) < halfW + ball.radius;
        const isOverlapY = Math.abs(localY) < halfH + ball.radius;

        if (!isOverlapX || !isOverlapY) {
            return;
        }

        const overlapX = halfW + ball.radius - Math.abs(localX);
        const overlapY = halfH + ball.radius - Math.abs(localY);

        const localV = this._worldToLocalVector(ball.vx, ball.vy);
        const localVx = localV.x;
        const localVy = localV.y;

        if (overlapX < overlapY) {
            this._resolveCollisionX(ball, overlapX, localX, localVx, localVy, energyLoss);
        } else {
            this._resolveCollisionY(ball, overlapY, localY, localVx, localVy, energyLoss);
        }
    }

    _worldToLocal(x, y) {
        const dx = x - this._x;
        const dy = y - this._y;
        const cos = Math.cos(-this._angle);
        const sin = Math.sin(-this._angle);
        return {
            x: dx * cos - dy * sin,
            y: dx * sin + dy * cos
        };
    }

    _worldToLocalVector(vx, vy) {
        const cos = Math.cos(-this._angle);
        const sin = Math.sin(-this._angle);
        return {
            x: vx * cos - vy * sin,
            y: vx * sin + vy * cos
        };
    }

    _localToWorldVector(localVx, localVy) {
        const cos = Math.cos(this._angle);
        const sin = Math.sin(this._angle);
        return {
            x: localVx * cos - localVy * sin,
            y: localVx * sin + localVy * cos
        };
    }

    _resolveCollisionX(ball, overlapX, localX, localVx, localVy, energyLoss) {
        const signX = localX > 0 ? 1 : -1;

        const correction = overlapX * signX;
        ball.x += correction * Math.cos(this._angle);
        ball.y += correction * Math.sin(this._angle);

        const newLocalVx = -localVx * energyLoss;

        const worldV = this._localToWorldVector(newLocalVx, localVy);
        ball.vx = worldV.x;
        ball.vy = worldV.y;
    }

    _resolveCollisionY(ball, overlapY, localY, localVx, localVy, energyLoss) {
        if (localY > 0) {
            ball.x += overlapY * (-Math.sin(this._angle));
            ball.y += overlapY * Math.cos(this._angle);
        } else {
            ball.x -= overlapY * (-Math.sin(this._angle));
            ball.y -= overlapY * Math.cos(this._angle);
        }

        const newLocalVx = localVx;
        const newLocalVy = -localVy * energyLoss;

        const worldV = this._localToWorldVector(newLocalVx, newLocalVy);
        ball.vx = worldV.x;
        ball.vy = worldV.y;
    }
}
