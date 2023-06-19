pipeline {
    agent none
    triggers {
        cron('H 2 * * 1-5') // nightly around 2 am
        pollSCM('H/5 * * * *') // check git every five minutes
    }
        parameters {
        string(name: 'BC_LIB', defaultValue: 'develop', description: 'braincloud-cpp branch')
        string(name: 'TEST_NAME', defaultValue: 'Auth', description: 'test filter')
    }
        stages {
            
        stage('Unit Tests on Mac') {
            agent {
                label 'clientUnit'
            }
            environment {
			    PATH = "/Applications/CMake.app/Contents/bin:/usr/local/bin:${env.PATH}"
  			}
            steps {
                deleteDir()
                checkout([$class: 'GitSCM', branches: [[name: '*/${BC_LIB}']], extensions: [[$class: 'SubmoduleOption', disableSubmodules: false, parentCredentials: false, recursiveSubmodules: true, reference: '', trackingSubmodules: false]], userRemoteConfigs: [[url: 'https://github.com/getbraincloud/braincloud-cpp.git']]])				
                sh 'cp ~/bin/test_ids_internal.txt autobuild/ids.txt'
			    sh 'autobuild/runtests.sh ${TEST_NAME}'
            }
            post {
	      		completed {
    	    		junit testResults: 'build/tests/results.xml', skipPublishingChecks: true
      			}
            }
        }        
            
        stage('Unit Tests on Linux') {
            agent {
                label '"Linux Build Agent (.41)"'
            }
            environment {
			    PATH = "/Applications/CMake.app/Contents/bin:/usr/local/bin:${env.PATH}"
  			}
            steps {
                deleteDir()
                checkout([$class: 'GitSCM', branches: [[name: '*/${BC_LIB}']], extensions: [[$class: 'SubmoduleOption', disableSubmodules: false, parentCredentials: false, recursiveSubmodules: true, reference: '', trackingSubmodules: false]], userRemoteConfigs: [[url: 'https://github.com/getbraincloud/braincloud-cpp.git']]])				
                sh 'cp ~/bin/test_ids_internal.txt autobuild/ids.txt'
			    sh 'autobuild/runtests.sh ${TEST_NAME}'
            }
            post {
	      		completed {
    	    		junit testResults: 'build/tests/results.xml', skipPublishingChecks: true
      			}
            }
        }
                    
         stage('Tests on Windows') {
            agent {
                label 'Windows Build Agent (.34)'
            }
            steps {
                deleteDir()
                checkout([$class: 'GitSCM', branches: [[name: '*/${BC_LIB}']], extensions: [[$class: 'SubmoduleOption', disableSubmodules: false, parentCredentials: false, recursiveSubmodules: true, reference: '', trackingSubmodules: false]], userRemoteConfigs: [[url: 'https://github.com/getbraincloud/braincloud-cpp.git']]])				
            	bat 'copy /Y C:\\Users\\buildmaster\\bin\\test_ids_internal.txt autobuild\\ids.txt'
            	bat 'autobuild\\runtests.bat %TEST_NAME%'
            }
            post {
	      		completed {
    	    		junit testResults: 'build/tests/results.xml', skipPublishingChecks: true
      			}
  			}	 
        }
        // end stages
    }
    // end pipeline
}