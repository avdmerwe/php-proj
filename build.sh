#!/bin/bash
# Build script for PHP PROJ Extension

set -e

PACKAGE_NAME="php-proj"
VERSION="2.0.0"

show_help() {
    echo "PHP PROJ Extension Build Script"
    echo "Usage: $0 [command]"
    echo ""
    echo "Commands:"
    echo "  build     - Configure and build the extension"
    echo "  test      - Run the test suite"
    echo "  leak      - Run the test suite with memory leak detection"
    echo "  clean     - Clean build artifacts"
    echo "  distclean - Remove all generated files (pristine state)"
    echo "  debian    - Build Debian package"
    echo "  release   - Create a new release (builds, tests, and creates release archive)"
    echo "  tag       - Create and push a git tag for release"
    echo "  push      - Push changes to GitHub"
    echo "  help      - Show this help"
    echo ""
}

do_build() {
    echo "Building PHP PROJ extension..."
    if [ ! -f configure ]; then
        echo "Running phpize..."
        phpize 2>&1 | grep -v "warning:" || true
    fi
    if [ ! -f config.status ]; then
        echo "Configuring..."
        ./configure > build_config.log 2>&1
        if [ $? -ne 0 ]; then
            echo "Configuration failed. Check build_config.log for details."
            exit 1
        fi
        echo "Configuration completed"
    fi
    echo "Compiling..."
    make > build_compile.log 2>&1
    if [ $? -ne 0 ]; then
        echo "Compilation failed. Check build_compile.log for details."
        exit 1
    fi
    echo "Build completed successfully"
    if [ -f modules/proj.so ]; then
        echo "Extension built: modules/proj.so"
    fi
}

do_test() {
    echo "Running test suite..."
    if [ ! -f config.status ]; then
        echo "Extension not built yet, building first..."
        do_build
    fi
	php run-tests.php -q -v -d extension=/home/abz/work/code/php-proj/php-proj-2.0.0/modules/proj.so | grep -E "(PASS|FAIL|Tests passed|Tests failed)" || true
    echo "Test suite completed"
}

do_leak() {
    echo "Running test suite with memory leak detection..."
    if [ ! -f config.status ]; then
        echo "Extension not built yet, building first..."
        do_build
    fi
	php run-tests.php -m -q -v -d extension=/home/abz/work/code/php-proj/php-proj-2.0.0/modules/proj.so #| grep -E "(PASS|FAIL|Tests passed|Tests failed)" || true
    echo "Test suite with memory leak detection completed"
}

do_clean() {
    echo "Cleaning build artifacts..."
    if [ -f Makefile ]; then
        make clean || true
    fi
    rm -f modules/*.so modules/*.la 2>/dev/null || true
    rm -f src/*.lo src/*.o src/*.dep 2>/dev/null || true
    rm -rf src/.libs 2>/dev/null || true
    echo "Clean completed"
}

do_distclean() {
    echo "Performing distclean - removing all generated files..."
    
    # Clean debian build artifacts
    if [ -f debian/rules ]; then
        debian/rules clean 2>/dev/null || true
    fi
    rm -rf debian/${PACKAGE_NAME}/ debian/${PACKAGE_NAME}-dev/
    rm -f debian/*.substvars debian/debhelper-build-stamp debian/files
    rm -f debian/autoreconf.before debian/autoreconf.after
    
    # Clean autotools and build files
    if [ -f Makefile ]; then
        make distclean || true
    fi
    rm -f config.status config.log config.cache config.nice config.h libtool
    rm -f Makefile Makefile.fragments Makefile.objects
    rm -f configure configure.ac aclocal.m4 install-sh missing ltmain.sh
    rm -f config.guess config.sub compile depcomp
    rm -rf autom4te.cache/ build/ include/ modules/ libs/ .libs/
	rm -f config.h.in
    
    # Clean source artifacts
    find src -name "*.lo" -delete 2>/dev/null || true
    find src -name "*.o" -delete 2>/dev/null || true
    find src -name "*.dep" -delete 2>/dev/null || true
    find src -name "*.la" -delete 2>/dev/null || true
    rm -rf src/.libs/ 2>/dev/null || true
    
    # Clean test artifacts
    rm -f tests/*.out tests/*.diff tests/*.exp tests/*.log tests/*.sh 2>/dev/null || true
    rm -f tmp-php.ini test_summary.* 2>/dev/null || true
	rm -f run-tests.php 2>/dev/null || true
    
    # Clean package artifacts
    rm -f ../${PACKAGE_NAME}_${VERSION}*.deb ../${PACKAGE_NAME}-dbgsym_${VERSION}*.ddeb 2>/dev/null || true
    rm -f ../${PACKAGE_NAME}_${VERSION}*.tar.xz ../${PACKAGE_NAME}_${VERSION}*.dsc 2>/dev/null || true
    rm -f ../${PACKAGE_NAME}_${VERSION}*.changes ../${PACKAGE_NAME}_${VERSION}*.buildinfo 2>/dev/null || true
    
    # Clean logs
    rm -f build*.log lintian*.log build_config.log build_compile.log build_debian.log 2>/dev/null || true
    
    echo "Directory is now in pristine state"
}

do_debian() {
    echo "Building Debian package..."
    if [ ! -d debian ]; then
        echo "Error: debian/ directory not found"
        exit 1
    fi
    
    dpkg-buildpackage -rfakeroot -uc -us > build_debian.log 2>&1
    if [ $? -ne 0 ]; then
        echo "Debian package build failed. Check build_debian.log for details."
        exit 1
    fi
    
    echo "Debian package build completed"
    echo "Generated files:"
    ls -la ../${PACKAGE_NAME}_${VERSION}*.deb ../${PACKAGE_NAME}-dbgsym_${VERSION}*.ddeb 2>/dev/null || true
}

do_release() {
    echo "Creating release ${VERSION}..."
    
    # Clean and build
    echo "Cleaning build artifacts..."
    do_clean
    
    echo "Building extension..."
    do_build
    
    echo "Running tests..."
    do_test
    
    # Create release directory
    RELEASE_DIR="${PACKAGE_NAME}-${VERSION}"
    echo "Creating release archive..."
    
    # Create temporary directory for release
    rm -rf /tmp/${RELEASE_DIR}
    mkdir -p /tmp/${RELEASE_DIR}
    
    # Copy files (excluding build artifacts and git)
    cp -r src tests examples *.md *.sh *.c *.m4 config.w32 /tmp/${RELEASE_DIR}/ 2>/dev/null || true
    cp -r debian /tmp/${RELEASE_DIR}/ 2>/dev/null || true
    
    # Create tarball
    cd /tmp
    tar -czf ${RELEASE_DIR}.tar.gz ${RELEASE_DIR}
    cd - > /dev/null
    
    # Move tarball to current directory
    mv /tmp/${RELEASE_DIR}.tar.gz .
    rm -rf /tmp/${RELEASE_DIR}
    
    echo "Release archive created: ${RELEASE_DIR}.tar.gz"
    echo ""
    echo "Next steps:"
    echo "1. Review the release archive"
    echo "2. Run './build.sh tag' to create and push git tag"
    echo "3. Upload ${RELEASE_DIR}.tar.gz to GitHub releases"
}

do_tag() {
    echo "Creating git tag for version ${VERSION}..."
    
    # Check if git is initialized
    if [ ! -d .git ]; then
        echo "Error: Not a git repository. Run 'git init' first."
        exit 1
    fi
    
    # Check for uncommitted changes
    if ! git diff-index --quiet HEAD --; then
        echo "Error: You have uncommitted changes. Commit them first."
        exit 1
    fi
    
    # Create tag
    TAG="v${VERSION}"
    echo "Creating tag ${TAG}..."
    git tag -a ${TAG} -m "Release ${TAG}"
    
    echo "Tag created: ${TAG}"
    echo "To push the tag to GitHub, run: git push origin ${TAG}"
    echo "Or use './build.sh push' to push everything"
}

do_push() {
    echo "Pushing to GitHub..."
    
    # Check if git is initialized
    if [ ! -d .git ]; then
        echo "Error: Not a git repository. Run 'git init' first."
        exit 1
    fi
    
    # Check if remote is configured
    if ! git remote get-url origin > /dev/null 2>&1; then
        echo "Error: No remote 'origin' configured."
        echo "Add remote with: git remote add origin https://github.com/YOUR-USERNAME/REPO-NAME.git"
        exit 1
    fi
    
    # Push commits
    echo "Pushing commits..."
    git push origin main
    
    # Push tags
    echo "Pushing tags..."
    git push --tags
    
    echo "Push completed"
}

# Main command processing
case "${1:-help}" in
    build)
        do_build
        ;;
    test)
        do_test
        ;;
    leak)
        do_leak
        ;;
    clean)
        do_clean
        ;;
    distclean)
        do_distclean
        ;;
    debian)
        do_debian
        ;;
    release)
        do_release
        ;;
    tag)
        do_tag
        ;;
    push)
        do_push
        ;;
    help|--help|-h)
        show_help
        ;;
    *)
        echo "Unknown command: $1"
        echo ""
        show_help
        exit 1
        ;;
esac
