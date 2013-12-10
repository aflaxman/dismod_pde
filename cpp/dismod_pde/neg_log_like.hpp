# ifndef DISMOD4_NEG_LOG_LIKE
# define DISMOD4_NEG_LOG_LIKE

# include <cmath>
# include <dismod_pde/define.hpp>

DISMOD4_BEGIN_NAMESPACE
/*!
\file neg_log_like.hpp 
Include file for \ref neg_log_like().
*/
double neg_log_like(
     double scaled               ,
     Likelihood_Enum likelihood
);

DISMOD4_END_NAMESPACE

# endif
