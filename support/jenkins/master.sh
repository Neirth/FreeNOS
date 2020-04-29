#!/bin/bash
#
# Copyright (C) 2019 Niek Linnenbank
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

# Include common functions
source common.sh

# Disable interactive dialogs for APT
export DEBIAN_FRONTEND=noninteractive

# Set hostname
echo master > /etc/hostname

# Auto select mirror
sed -i 's/us.archive.ubuntu.com/nl.archive.ubuntu.com/' /etc/apt/sources.list

# Remove hardcoded DNS servers
cat /etc/netplan/01-netcfg.yaml | grep -v 'nameservers:' | grep -v 'addresses: ' > /etc/netplan/01-netcfg.yaml.new
mv /etc/netplan/01-netcfg.yaml.new /etc/netplan/01-netcfg.yaml
netplan apply
sleep 5

# Update system to latest patches
run_command_retry "apt-get update"
run_command_retry "apt-get dist-upgrade -y"

# Install jenkins dependencies
run_command_retry "apt-get install -y git default-jre daemon"

# Add jenkins repository
run_command_retry "wget -q -O jenkins.io.key http://pkg.jenkins.io/debian/jenkins.io.key"
apt-key add jenkins.io.key
echo deb http://pkg.jenkins.io/debian-stable binary/ > /etc/apt/sources.list.d/jenkins.list
run_command_retry "apt-get update"

# Prevent automatic start of jenkins
if [ ! -e /usr/bin/daemon.bak ] ; then
    mv /usr/bin/daemon /usr/bin/daemon.bak
fi
cp /bin/true /usr/bin/daemon

# Install jenkins
run_command_retry "apt-get install -y jenkins"

# Restore daemon program
mv /usr/bin/daemon.bak /usr/bin/daemon

# Disable the setup wizard
if [ "`grep runSetupWizard /etc/default/jenkins|wc -l`" -eq "0" ] ; then
   echo 'JAVA_ARGS="$JAVA_ARGS -Djenkins.install.runSetupWizard=false"' >> /etc/default/jenkins
fi

source /etc/default/jenkins

# Add default admin user
mkdir -p $JENKINS_HOME/init.groovy.d/
cat > $JENKINS_HOME/init.groovy.d/01-basic-security.groovy << EOF
#!groovy

import jenkins.model.*
import hudson.util.*;
import jenkins.install.*;
import hudson.security.*
import jenkins.security.s2m.AdminWhitelistRule

def instance = Jenkins.getInstance()

def hudsonRealm = new HudsonPrivateSecurityRealm(false)
hudsonRealm.createAccount("admin", "admin")
instance.setSecurityRealm(hudsonRealm)

def strategy = new FullControlOnceLoggedInAuthorizationStrategy()
instance.setAuthorizationStrategy(strategy)
instance.save()

Jenkins.instance.getInjector().getInstance(AdminWhitelistRule.class).setMasterKillSwitch(false)
EOF

# Override number of executors, if cpu count is specified
if [ ! -z "$SLAVE_CPUS" ] ; then
    sed -i "s/<numExecutors>4<\/numExecutors>/<numExecutors>$SLAVE_CPUS<\/numExecutors>/" ~vagrant/*.node.xml
fi

# Add configuration files to jenkins
mkdir -p $JENKINS_HOME/jobs
mkdir -p $JENKINS_HOME/jobs/FreeNOS-ubuntu1804
mkdir -p $JENKINS_HOME/jobs/FreeNOS-freebsd12
mkdir -p $JENKINS_HOME/nodes
mkdir -p $JENKINS_HOME/nodes/ubuntu1804
mkdir -p $JENKINS_HOME/nodes/freebsd12
mkdir -p $JENKINS_HOME/secrets
mv ~vagrant/master.key $JENKINS_HOME/secrets/
mv ~vagrant/hudson.util.Secret $JENKINS_HOME/secrets/
mv ~vagrant/credentials.xml $JENKINS_HOME/
mv ~vagrant/ubuntu1804.job.xml $JENKINS_HOME/jobs/FreeNOS-ubuntu1804/config.xml
mv ~vagrant/ubuntu1804.node.xml $JENKINS_HOME/nodes/ubuntu1804/config.xml
mv ~vagrant/freebsd12.job.xml $JENKINS_HOME/jobs/FreeNOS-freebsd12/config.xml
mv ~vagrant/freebsd12.node.xml $JENKINS_HOME/nodes/freebsd12/config.xml

# Ensure permissions are set properly for jenkins
chown -R $JENKINS_USER:$JENKINS_GROUP $JENKINS_HOME

# Restart jenkins
/etc/init.d/jenkins stop
/etc/init.d/jenkins start
sleep 30

# Download jenkins client library
run_command_retry "wget -q http://localhost:$HTTP_PORT/jnlpJars/jenkins-cli.jar -O jenkins-cli.jar"

# Install jenkins plugins
run_command_retry "java -jar jenkins-cli.jar -s http://localhost:$HTTP_PORT/ -auth admin:admin install-plugin git"
run_command_retry "java -jar jenkins-cli.jar -s http://localhost:$HTTP_PORT/ -auth admin:admin install-plugin matrix-project"
run_command_retry "java -jar jenkins-cli.jar -s http://localhost:$HTTP_PORT/ -auth admin:admin install-plugin matrix-combinations-parameter"
run_command_retry "java -jar jenkins-cli.jar -s http://localhost:$HTTP_PORT/ -auth admin:admin install-plugin nodelabelparameter"
run_command_retry "java -jar jenkins-cli.jar -s http://localhost:$HTTP_PORT/ -auth admin:admin install-plugin ws-cleanup"
run_command_retry "java -jar jenkins-cli.jar -s http://localhost:$HTTP_PORT/ -auth admin:admin install-plugin junit"
run_command_retry "java -jar jenkins-cli.jar -s http://localhost:$HTTP_PORT/ -auth admin:admin install-plugin ssh-slaves"

# Restart jenkins to load the new plugins
/etc/init.d/jenkins stop
/etc/init.d/jenkins start
