class ImageLoader {
    constructor() {
        this.images = new Map();
        this.loadedCount = 0;
    }

    loadImage(src) {
        return new Promise((resolve, reject) => {
            const img = new Image();

            img.onload = () => {
                this.images.set(src, img);
                this.loadedCount++;
                resolve(img);
            };

            img.onerror = () => {
                console.error(`Ошибка загрузки: ${src}`);
                reject(new Error(`Не удалось загрузить: ${src}`));
            };

            img.src = src;
        });
    }

    loadAllImages(imagePaths) {
        const promises = imagePaths.map(src => this.loadImage(src));
        return Promise.all(promises);
    }

    getImage(src) {
        return this.images.get(src);
    }
}
