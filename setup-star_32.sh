# source setup-star.sh

_this_script="${BASH_SOURCE[0]:-}"
export STAR="$(cd "$(dirname $_this_script)" > /dev/null && pwd)"

echo "STAR=$STAR"

export USE_64BITS=0

source /afs/rhic/star/packages/ROOT/5.34.38/.sl73_gcc485/root/bin/thisroot.sh 

export CERN=/cern
export CERN_LEVEL=pro
export CERN_ROOT=$CERN/$CERN_LEVEL

#ENV ROOTSYS=/usr/local
#ENV ROOT_LEVEL=5.34.38

#export SITE=generic
export OPTSTAR=/afs/rhic/opt/star/sl73_gcc485/

export STAR_CVS_REF=ZZZ

#export STARCMPL=gcc
#export STARCMPLC=gcc

#export STAR_HOST_SYS=sl73_gcc485

export STAR_HOST_SYS=sl88_gcc789

#export STAR_BASE_PATH=
#export STAR_PATH=/tmp/star-cvs
#export STAR=/tmp/star-cvs/${STAR_CVS_REF}
#export STAR=/star/u/smirnovd/gpfs/test-build-root6
#export STAR=/star/u/smirnovd/gpfs/star-sw

export NODEBUG=yes
export STAR_LIB=${STAR}/.${STAR_HOST_SYS}/LIB
export STAR_BIN=${STAR}/.${STAR_HOST_SYS}/BIN
#export STAR_LIB=${STAR}/.${STAR_HOST_SYS}/lib
#export STAR_BIN=${STAR}/.${STAR_HOST_SYS}/bin

export STAR_SCRIPTS=${STAR}/scripts
export STAR_CGI=${STAR}/cgi
export STAR_MGR=${STAR}/mgr
export STAR_PAMS=${STAR}/pams
export STAR_LEVEL=${STAR_CVS_REF}
export STAR_VERSION=${STAR_CVS_REF}

export STAR_SYS=x86_sl7

#ENV PATH=.:${STAR_BIN}:$STAR_MGR:$STAR_SCRIPTS:$STAR_CGI:${CERN_ROOT}/bin:${PATH}
#ENV LD_LIBRARY_PATH=${STAR_LIB}:/usr/local/lib:${LD_LIBRARY_PATH}

export PATH=${CERN_ROOT}/bin:${STAR_BIN}:$STAR_MGR:${PATH}
#export LD_LIBRARY_PATH=$STAR_LIB:${LD_LIBRARY_PATH}
