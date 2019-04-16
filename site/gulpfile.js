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
const through = require('through2');
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
        assets: {
            base: path.resolve('src/assets'),
            files: path.resolve('src/assets/**/*'),
        },
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
        files: path.resolve('dist/**/*'),
    },
    temp: {
        base: path.resolve('.tmp'),
        partials: path.resolve('.tmp/**/*.hbs'),
    },
    target: {
        base: path.resolve('../sonoff/data'),
    }
};

// helpers
function fontMatterFileName(){
    return through.obj(function(file, enc, next) {
        if(file && file.data && file.data.filename){
            file.path = path.join(path.dirname(file.path), file.data.filename);
        }
        next(null, file);
    });
}

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
        .pipe(
            frontMatter({
                property: 'data'
            })
        )
        .pipe(extname())
        .pipe(fontMatterFileName())
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

task('copy', () => {
    return src([dir.dist.files, dir.src.assets.files])
        .pipe(dest(dir.target.base));
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
exports.build = series(['clean', 'js', 'scss', 'html']);
exports.default = exports.install = series(['clean', 'js', 'scss', 'html', 'copy']);
