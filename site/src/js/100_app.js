let app = {
    runState: 'stop',
    $up: null,
    $down: null,
    timer: null,
    interval: 40
};

function callBed(value) {
    $.get(value)
        .fail((e) => {
            //TODO
        });
}

function onTimer() {
    if (app.state === 'up') {
        callBed('u');
    } else if (app.state === 'down') {
        callBed('d');
    } else {
        stopTimer();
    }
}

function startTimer() {
    if (!app.timer) {
        onTimer();
        app.timer = setInterval(onTimer, app.interval);
    }
}

function stopTimer() {
    if (!app.timer) {
        clearInterval(app.timer);
        app.timer = null;
    }
}

function setState(state) {
    if (state === 'up') {
        app.$up.addClass('active');
        app.state = 'up';
        startTimer();
    } else if (state === 'down') {
        app.$down.addClass('active');
        app.state = 'down';
        startTimer();
    } else {
        app.$up.removeClass('active');
        app.$down.removeClass('active');
        app.state = 'stop';
        stopTimer();
    }
}

function initButton(id, state) {
    let $btn = $('#' + id);
    let s = state || id;
    $btn.on('touchstart mousedown', () => {
        setState(s);
    });

    $btn.on('touchmove', (e) => {
        let x = e.targetTouches[0].pageX;
        let y = e.targetTouches[0].pageY;
        // let rect = app.$up.clientRect();
        //
        // if (x < rect.left || x > rect.right || y < rect.top || y > rect.bottom) {
        //     setState('stop');
        // }
    });

    $btn.on('touchend touchcancel mouseleave mouseup', () => {
        setState('stop');
    });

    return $btn;
}

function init() {
    app.$up = initButton('up');
    app.$down = initButton('down');
}

$(document).ready(() => init());
