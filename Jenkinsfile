pipeline {
    agent none
    options {
        disableConcurrentBuilds()
    }
    stages {

        

        stage('Build Linux') {
            agent {
                dockerfile {
                    filename 'Dockerfile_linux'
                    additionalBuildArgs '-t boden_linux'
                    args '--volume ${WORKSPACE}:/boden -w /boden'
                }
            }
            steps {
                sh 'python build.py prepare --platform linux --build-system make'
                sh 'python build.py build --platform linux --config Release'
                
                stash includes: 'build/**/*', name: 'boden_linux_builddir'
            }
        }

        stage('Test Linux') {
            agent {
                dockerfile {
                    filename 'Dockerfile_linux'
                    additionalBuildArgs '-t boden_linux'
                    args '--volume ${WORKSPACE}:/boden -w /boden'
                }
            }
            steps {
                unstash 'boden_linux_builddir'

                sh 'mkdir -p testresults'

                sh 'python build.py --platform linux --config Release --module testboden -- run --out testresults/linux_testboden.xml --reporter junit --reporter console --print-level 2 || true'
                junit "testresults/linux_testboden.xml"

                sh 'xvfb-run --server-args=\'-screen 0, 1024x768x16\' -- python build.py --platform linux --config Release --module testbodenui -- run --out testresults/linux_testbodenui.xml --reporter junit --reporter console --print-level 2 || true'
                junit "testresults/linux_testbodenui.xml"

                sh 'xvfb-run --server-args=\'-screen 0, 1024x768x16\' -- python build.py --platform linux --config Release --module testbodentiming -- run --out testresults/linux_testbodentiming.xml --reporter junit --reporter console --print-level 2 || true'
                junit "testresults/linux_testbodentiming.xml"
            }
        }

        
        stage('Build webems') {
            agent {
                dockerfile {
                    filename 'Dockerfile_webems'
                    additionalBuildArgs  '-t boden_webems'
                    args '--volume ${WORKSPACE}:/boden -w /boden'
                }
            }

            steps {
                sh 'python build.py prepare --platform webems --build-system make'
                sh 'python build.py build --platform webems --config Release'
                stash includes: 'build/**/*', name: 'boden_webems_builddir'
            }
        }

        stage('Test webems') {
            agent {
                dockerfile {
                    filename 'Dockerfile_webems'
                    additionalBuildArgs  '-t boden_webems'
                    args '--volume ${WORKSPACE}:/boden -w /boden'
                    reuseNode true
                }
            }
            options {
                /* Abort the tests if they run too long. We have had cases where
                   the run call hung indefinitely.*/
                timeout(time: 2, unit: 'HOURS') 
            }

            /* Note that we run the Release config. It is much faster and many browsers cannot even
               deal with the huge Javascript files of the debug build.*/
            steps {
            	unstash 'boden_webems_builddir'
                sh 'mkdir -p testresults'    
                
                /* The "|| true" at the end is there to ensure that the exit code of the command is ignored
                   (which will indicate an error if tests failed) and the script will continue.
                   Note that the junit command on the next line will fail if the test did not start or crashed, since then there
                   will be no results file.*/

                /* Also note that in addition to the junit reporter we also use the console reporter.
                   That is actually pretty essential, because the junit reporter only writes output at the very end
                   of the test. We have encountered cases where the output pipe times out and the browser tab crashes
                   when we do not write any data to it for a few minutes. This seems to be a bug of some kind (since
                   emrun is configured to NOT have a silence timeout by default), but for the time being we work around
                   it by always having some status output (via the console reporter).
                   Note that the console reporter prints to stderr and the junit reporter to stdout. So they can be combined
                   without interfering with each other.
                   */
                   
                sh "xvfb-run --server-args=\'-screen 0, 1024x768x16\' -- python build.py --platform webems --config Release --module testboden --run-output-file testresults/webems_testboden.xml -- run --reporter junit --reporter console --print-level 2 || true"
                junit "testresults/webems_testboden.xml"

                sh "xvfb-run --server-args=\'-screen 0, 1024x768x16\' -- python build.py --platform webems --config Release --module testbodenui --run-output-file testresults/webems_testbodenui.xml -- run --reporter junit --reporter console --print-level 2  || true"
                junit "testresults/webems_testbodenui.xml"

                sh "xvfb-run --server-args=\'-screen 0, 1024x768x16\' -- python build.py --platform webems --config Release --module testbodentiming --run-output-file testresults/webems_testbodentiming.xml -- run --reporter junit --reporter console --print-level 2 || true"
                junit "testresults/webems_testbodentiming.xml"
            }
        }

        
        stage('Build Android') {
            agent {
                dockerfile {
                    filename 'Dockerfile_android'
                    additionalBuildArgs  '-t boden_android'
                    args '--volume ${WORKSPACE}:/boden -w /boden'
                }
            }

            steps {
                
                sh 'python build.py prepare --platform android --arch x86_64 --build-system AndroidStudio'
                sh 'python build.py build --platform android --arch x86_64 --config Debug'
                stash includes: 'build/**/*', name: 'boden_android_builddir'
            }
        }

        
        /* Disabled tests because we were unable to get the android emulator to run in AWS.
           Neither --device /dev/kvm nor --privileged seems to work.
        stage('Test Android') {
            agent {
                dockerfile {
                    filename 'Dockerfile_android'
                    additionalBuildArgs  '-t boden_android'
                    args '--volume ${WORKSPACE}:/boden -w /boden'
                    reuseNode true
                }
            }

            steps {
                unstash 'boden_android_builddir'

                sh 'mkdir -p testresults'   

                sh 'python build.py --platform android --arch x86_64 --config Debug --module testboden --run-output-file testresults/android_testboden.xml --run-android-fetch-output-from {DATA_DIR}/testresults.xml -- run --out {DATA_DIR}/testresults.xml --reporter junit --reporter console --force-exit-at-end --print-level 2 || true'
                junit "testresults/android_testboden.xml"

                sh 'python build.py --platform android --arch x86_64 --config Debug --module testbodenui --run-output-file testresults/android_testbodenui.xml --run-android-fetch-output-from {DATA_DIR}/testresults.xml -- run --out {DATA_DIR}/testresults.xml --reporter junit --reporter console --force-exit-at-end --print-level 2 || true'
                junit "testresults/android_testbodenui.xml"

                sh 'python build.py --platform android --arch x86_64 --config Debug --module testbodentiming --run-output-file testresults/android_testbodentiming.xml --run-android-fetch-output-from {DATA_DIR}/testresults.xml -- run --out {DATA_DIR}/testresults.xml --reporter junit --reporter console --force-exit-at-end --print-level 2 || true'
                junit "testresults/android_testbodentiming.xml"
            }
        }*/


        stage('Build macOS') {
            agent { label 'macOS' }
            steps {
                sh 'python build.py prepare --platform mac --build-system Xcode'
                sh 'python build.py build --platform mac --config Release --module testboden'
            }
        }

        /* tests for ios should be called with this commandline:
        sh 'mkdir -p testresults'
        sh 'python build.py --platform ios --config Debug --module testboden --run-output-file testresults/ios_testboden.xml -- run --reporter junit --reporter console --force-exit-at-end --print-level 2 || true'
        junit "testresults/ios_testboden.xml"
        */


    }
}
