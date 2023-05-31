pipeline {
    agent none
    stages {

        stage('Tests on Mac') {
            agent {
                label 'clientUnit'
            }
            environment {
			    PATH = "/Applications/CMake.app/Contents/bin:/usr/local/bin:${env.PATH}"
  			}
            steps {
            	echo "Mac..."
				sh 'cp ~/bin/test_ids_internal.txt ids.txt'
				sh 'autobuild/runtests.sh ${TEST_NAME}'
            }
            post {
	      		always {
    	    		junit 'build/tests/results.xml'
      			}
  			}	 
        }
        
        stage('Tests on Linux') {
            agent { 
                label '"Linux Build Agent (.41)"'
            }
            environment {
			    PATH = "/usr/bin:${env.PATH}"
  			}
  			steps { 
            	echo 'Linux...'
				sh 'cp ~/bin/test_ids_internal.txt ids.txt'
				sh 'bash autobuild/runtests.sh ${TEST_NAME}'
            }
            post {
	      		always {
    	    		junit 'build/tests/results.xml'
      			}
  			}	 
        }
        
         stage('Tests on Windows') {
            agent {
                label 'Windows Build Agent (.34)'
            }
            steps {
            	echo "Windows..."
            	bat 'copy /Y C:\\Users\\buildmaster\\bin\\test_ids_internal.txt ids.txt'
            	bat 'autobuild\\runtests.bat %TEST_NAME%'
            }
            post {
	      		always {
    	    		junit 'build/tests/results.xml'
      			}
  			}	 
        }
        
        stage('Package on Mac') {
            agent {
                label 'clientUnit'
            }
            environment {
			    PATH = "/Applications/CMake.app/Contents/bin:/usr/local/bin:${env.PATH}"
  			}
            steps {
        				sh 'autobuild/build_apple_unified.sh 1.2.3 .'
        	}
        	}
    }
}