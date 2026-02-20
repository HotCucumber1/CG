const APP = {
    Width: window.innerWidth,
    Height: window.innerHeight
};

const AppMode = Object.freeze({
    Picture: 'Picture',
    Text: 'Text',
});

document.addEventListener('DOMContentLoaded', () => {
    const appSettings = {
        mode: AppMode.Picture
    }

    const canvas = document.createElement("canvas");
    canvas.width = APP.Width;
    canvas.height = APP.Height;
    document.body.appendChild(canvas);
    const context = canvas.getContext("2d")

    // const abc = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    const abc = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"

    const wordProvider = new WordProvider()
    const wordData = wordProvider.getRandomTopicAndWord()

    console.log(wordData)

    const hangmanModel = new HangmanModel()
    const lettersModel = new LettersModel(abc)
    const wordModel = new WordModel(wordData.word, wordData.topic)

    const hangmanView = new HangmanView(context, hangmanModel)
    const lettersView = new LettersView(context, lettersModel)
    const wordView = new WordView(context, wordModel)

    const appController = new AppController(
        lettersModel,
        hangmanModel,
        wordModel,
        lettersView,
        hangmanView,
        wordView,
    )

    mainLoop(context, appController)
});

/**
 * @param {CanvasRenderingContext2D} context
 * @param {AppController} appController
 */
function mainLoop(context, appController) {
    context.clearRect(0, 0, APP.Width, APP.Height)

    if (appController.isVictory()) {
        appController.drawVictory()
        return
    }

    if (!appController.isAlive()) {
        appController.drawGame()
        appController.drawLose()
        return
    }

    appController.drawGame()
    requestAnimationFrame(() => {
        mainLoop(context, appController)
    })
}
