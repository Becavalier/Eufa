const CDP = require('chrome-remote-interface');
const chromeLauncher = require('chrome-launcher');

// Launch chrome in headless mode;
chromeLauncher.launch({
    chromeFlags: ['--headless'],
    port: 9222
}).then(chrome => {
    CDP(client => {
        const { Runtime } = client;
        const code = ``;
        Runtime.evaluate({ expression: code }).then(result => {
            console.log(result);
            chrome.kill();
        });
    }).on('error', (err) => {
        // Cannot connect to the remote endpoint
        console.error(err);
    });
});
