const gulp = require('gulp');
const gzip = require('gulp-gzip');
const size = require('gulp-size');

gulp.task('build-fs-data', () => gulp.src(['./dist/**/*.*'])
  .pipe(gzip())
  .pipe(size())
  .pipe(gulp.dest('../data'))
);
