#include <Rcpp.h>
#include <distanceModel.hpp>

using namespace Rcpp;


distanceModel::distanceModel()
{
    numLocations=-1;
}

int distanceModel::getModelComponentType()
{
    return(LSS_DISTANCE_MODEL_TYPE);
}

void distanceModel::setPriorParameters(double _priorAlpha, double _priorBeta)
{
   priorAlpha = _priorAlpha;
   priorBeta = _priorBeta;
}

void distanceModel::addDistanceMatrix(NumericMatrix distMat)
{
    if (distMat.nrow() != distMat.ncol())
    {
        ::Rf_error("Distance matrix must be square.\n");
    }
    else if (numLocations != -1 && distMat.nrow() != (numLocations))
    {
        ::Rf_error("Dimension does not match previously added distance matrix\n");
    }
    Eigen::MatrixXd new_mat(distMat.nrow(), distMat.ncol());
    int i,j;
    for (i = 0; i < distMat.nrow(); i++)
    {
        for (j = 0; j < distMat.ncol(); j++)
        {
            new_mat(i,j) = distMat[i,j];
        }

    }

    dm_list.push_back(new_mat);
    numLocations = distMat.nrow();
}
void distanceModel::summary()
{
    Rcpp::Rcout << "Number of locations: " << numLocations << "\n";
    Rcpp::Rcout << "Number of distance structures: " << (dm_list.size()) << "\n";
}
distanceModel::~distanceModel()
{
    if (prot !=0 ){
        ::Rf_error("can't delete distanceModel, still being used.\n");
    }
}

int distanceModel::getNumDistanceMatrices()
{
    return(dm_list.size()); 
}

RCPP_MODULE(mod_distanceModel)
{
    using namespace Rcpp;
    class_<distanceModel>( "distanceModel" )
    .constructor()
    .method("addDistanceMatrix", &distanceModel::addDistanceMatrix)
    .method("summary", &distanceModel::summary)
    .method("setPriorParameters", &distanceModel::setPriorParameters)
    .property("numMatrices", &distanceModel::getNumDistanceMatrices, "Number of distict distance matrices.");
}


