// node/tools
const fs = require('fs');
const path = require('path');
const rimraf = require('rimraf');

// env
const env = require('@zebrajaeger/gulp-simple-env')({
    env: ['prod']
});

// gulp
const {task, src, dest, series, watch} = require('gulp');
const extreplace = require('gulp-ext-replace');

const hb = require('gulp-hb');
const hbHelpers = require('handlebars-helpers');
const hbLayouts = require('handlebars-layouts');
const frontMatter = require('gulp-front-matter');
const extname = require('gulp-extname');
const htmlmin = require('gulp-htmlmin');
const print = require('gulp-print').default;

const concat = require('gulp-concat');
const uglify = require('gulp-uglify-es').default;

const sass = require('gulp-sass');
const autoprefixer = require('gulp-autoprefixer');
const cssnano = require('gulp-cssnano');

// develop
const browserSync = require('browser-sync').create();

// constants
const dir = {
    base: path.resolve(),
    src: {
        base: path.resolve('src'),
        html: {
            base: path.resolve('src/html'),
            hbs: path.resolve('src/html/*.hbs'),
            data: path.resolve('src/html/data/**/*.json'),
            partials: path.resolve('src/html/partials/**/*.hbs'),
            watch: 'src/html/**/*'
        },
        scss: {
            base: path.resolve('src/scss'),
            filesToProcess: path.resolve('src/scss/*.scss'),
            allFiles: path.resolve('src/scss/**/*.scss'),
            watch: 'src/scss/**/*'
        },
        js: {
            base: path.resolve('src/js'),
            files: path.resolve('src/js/**/*.js'),
            watch: 'src/js/**/*'
        }
    },
    dist: {
        base: path.resolve('dist'),
        html:  path.resolve('dist/index.html'),
        h:  path.resolve('dist/index.h'),
    },
    temp: {
        base: path.resolve('.tmp'),
        partials: path.resolve('.tmp/**/*.hbs'),
    },
};

task('clean', cb => {
    rimraf.sync(dir.temp.base);
    rimraf.sync(dir.dist.base);
    cb();
});

task('scss', () => {
    return src(dir.src.scss.filesToProcess)
        .pipe(sass().on('error', sass.logError))
        .pipe(autoprefixer())
        .pipe(env.isProd(cssnano()))
        .pipe(extreplace('.css.hbs', '.css'))
        .pipe(env.isNotProd(print()))
        .pipe(dest(dir.temp.base))
});

task('js', () => {
    return src(dir.src.js.files)
        .pipe(concat('app.js.hbs'))
        .pipe(env.isProd(uglify()), print())
        .pipe(dest(dir.temp.base))
});

task('html', () => {
    //mkdirp.sync(dir.dist);
    return src(dir.src.html.hbs)
        .pipe(extname())
        .pipe(
            frontMatter({
                property: 'data'
            })
        )
        .pipe(
            hb({'cwd': dir.base})
                .data(dir.src.html.data)
                .data('timestamp', Date.now())
                .partials(dir.src.html.partials)
                .partials(dir.temp.partials)
                .helpers(hbHelpers)
                .helpers(hbLayouts)
        )
        .pipe(env.isProd(htmlmin({collapseWhitespace: true}), print()))
        .pipe(dest(dir.dist.base))
});

task('header', (cb)=>{
    let data = fs.readFileSync(dir.dist.html);
    let fd = fs.openSync(dir.dist.h, 'as');

    let prefix = Buffer.from('R"(');
    fs.writeSync(fd, prefix, 0, prefix.length, 0);

    fs.writeSync(fd, data, 0, data.length);

    let postfix = Buffer.from(')"');
    fs.writeSync(fd, postfix, 0, postfix.length, 0);

    fs.close(fd);
    cb();
});

task('bs-reload', cb => {
    browserSync.reload();
    cb();
});

task('dev', () => {
    browserSync.init({
        server: {
            baseDir: dir.dist.base
        }
    });

    watch(dir.src.scss.watch, series(['scss', 'html', 'bs-reload']));
    watch(dir.src.js.watch, series(['js', 'html', 'bs-reload']));
    watch(dir.src.html.watch, series(['html', 'bs-reload']));
});

// export tasks
exports.develop = series(['clean', 'js', 'scss', 'html', 'dev']);
exports.default = exports.build = series(['clean', 'js', 'scss', 'html', 'header']);
