const fs = require('fs');
const gulp = require('gulp');
const rollup = require('rollup');
const rollupUglify = require('rollup-plugin-uglify');
const rollupFilesize = require('rollup-plugin-filesize');
const gulpReplace = require('gulp-replace');
const gulpClean = require('gulp-clean');
const exec = require('child_process').exec;
const program = require('commander');


// Set base dir path;
const basedir = './dist/';


program
    .option('-m, --m [value]')
    .option('-w, --w [value]')
    .parse(process.argv);


gulp.task('default', () => {
    new Promise(function(resolve, reject) {
        fs.readFile(basedir + program.m, 'utf8', (err, data) => {
            gulp.src([basedir + program.w])
                .pipe(gulpReplace('// [ REPLACEMENT_PLACEHOLDER ]', data))
                // To fix for node context start
                .pipe(gulpReplace('require("fs")', 'null'))
                .pipe(gulpReplace('require("path")', 'null'))
                // To fix for node context end
                .pipe(gulp.dest('./dist/'))
                .on('end', resolve);
        });
    }).then(() => {
        return (async () => {
            const bundle = await rollup.rollup({
                entry: basedir + program.w,
                plugins: [
                    rollupUglify(),
                    rollupFilesize()
                ]
            });

            await bundle.write({
                format: 'umd',
                moduleName: 'Eufa',
                dest: './dist/eufa-umd.min.js',
                sourceMap: true
            });
        })();
    }).then(() => {
        // Clear files
        gulp.src([basedir + program.w], {read: false})
            .pipe(gulpClean());
    });
});


gulp.task('reset', () => {
    return gulp
        .src('test/node_modules/eufa', {read: false})
        .pipe(gulpClean({force: true}));
});


gulp.task('test', ['reset'], () => {
    return Promise.all([
        new Promise(function(resolve, reject) {
            gulp.src('package.json')
                .pipe(gulp.dest('test/node_modules/eufa/'))
                .on('end', resolve);
        }),
        new Promise(function(resolve, reject) {
            gulp.src('dist/eufa-umd.min.js')
                .pipe(gulp.dest('test/node_modules/eufa/dist'))
                .on('end', resolve);
        }),
        new Promise(function(resolve, reject) {
            gulp.src('dist/eufa-module.wasm')
                .pipe(gulp.dest('test/static'))
                .on('end', resolve);
        })
    ]).then(() => {
        exec('cd test && npm run dev', function (err, stdout, stderr) {
            console.log(stdout);
            console.error(stderr);
        }).stdout.pipe(process.stdout);
    });
});
