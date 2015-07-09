/* Generated from orogen/lib/orogen/templates/tasks/Task.hpp */

#ifndef ADAP_PARAMETERS_ESTIMATOR_ADAPMODELESTIMATION_TASK_HPP
#define ADAP_PARAMETERS_ESTIMATOR_ADAPMODELESTIMATION_TASK_HPP

#include "adap_parameters_estimator/AdapModelEstimationBase.hpp"
#include "adap_parameters_estimator/adap_parameters.hpp"
#include "base/commands/Joints.hpp"
#include "base/samples/RigidBodyState.hpp"

namespace adap_parameters_estimator {

    /*! \class AdapModelEstimatation 
     * \brief The task context provides and requires services. It uses an ExecutionEngine to perform its functions.
     * Essential interfaces are operations, data flow ports and properties. These interfaces have been defined using the oroGen specification.
     * In order to modify the interfaces you should (re)use oroGen and rely on the associated workflow.
     * normalized error of the real velocity and the estimated velocity of the adap_method
     * \details
     * The name of a TaskContext is primarily defined via:
     \verbatim
     deployment 'deployment_name'
         task('custom_task_name','adap_parameters_estimator::AdapModelEstimatation')
     end
     \endverbatim
     *  It can be dynamically adapted when the deployment is called with a prefix argument. 
     */
    class AdapModelEstimation : public AdapModelEstimationBase
    {
	friend class AdapModelEstimationBase;
    protected:

		// adaptive method
		adap_parameters_estimator::AdapParameters *adapParam;

		// Queue of forces and velocities samples
		std::queue<base::samples::RigidBodyState> queuePose;
		std::queue<double> queueStep;
		std::queue<base::samples::Joints> queueForces;
		int sizeQueue = 5;

		// Aux variables
		base::samples::RigidBodyState	lastPoseSample;
		base::samples::Joints			lastForceSample;
		double max_step;

		// Output model's parameters
		Parameters modelParameters;

        virtual void forces_samplesCallback(const base::Time &ts, const ::base::samples::Joints &forces_samples_sample);

        virtual void pose_samplesCallback(const base::Time &ts, const ::base::samples::RigidBodyState &pose_samples_sample);

        bool matchForce(const base::samples::RigidBodyState &pose_sample, base::samples::Joints &forces_output);

        bool getVectorForces(const base::samples::Joints &forces_sample, base::Vector6d &forces);
        void getVectorVelocities(const base::samples::RigidBodyState &pose_sample, base::Vector6d &velocities);

        void convertParameters(const base::Vector4d &parameters, Parameters &modelParameters, DOFS dof);

        void estimateStep(double &step);

        bool handleMeasurement(const base::samples::RigidBodyState &rbs_sample, double &step);
        bool handleMeasurement(const base::samples::Joints &force_sample);

    public:
        /** TaskContext constructor for AdapModelEstimatation
         * \param name Name of the task. This name needs to be unique to make it identifiable via nameservices.
         * \param initial_state The initial TaskState of the TaskContext. Default is Stopped state.
         */
        AdapModelEstimation(std::string const& name = "adap_parameters_estimator::AdapModelEstimatation");

        /** TaskContext constructor for AdapModelEstimatation 
         * \param name Name of the task. This name needs to be unique to make it identifiable for nameservices. 
         * \param engine The RTT Execution engine to be used for this task, which serialises the execution of all commands, programs, state machines and incoming events for a task. 
         * 
         */
        AdapModelEstimation(std::string const& name, RTT::ExecutionEngine* engine);

        /** Default deconstructor of AdapModelEstimatation
         */
	~AdapModelEstimation();

        /** This hook is called by Orocos when the state machine transitions
         * from PreOperational to Stopped. If it returns false, then the
         * component will stay in PreOperational. Otherwise, it goes into
         * Stopped.
         *
         * It is meaningful only if the #needs_configuration has been specified
         * in the task context definition with (for example):
         \verbatim
         task_context "TaskName" do
           needs_configuration
           ...
         end
         \endverbatim
         */
        bool configureHook();

        /** This hook is called by Orocos when the state machine transitions
         * from Stopped to Running. If it returns false, then the component will
         * stay in Stopped. Otherwise, it goes into Running and updateHook()
         * will be called.
         */
        bool startHook();

        /** This hook is called by Orocos when the component is in the Running
         * state, at each activity step. Here, the activity gives the "ticks"
         * when the hook should be called.
         *
         * The error(), exception() and fatal() calls, when called in this hook,
         * allow to get into the associated RunTimeError, Exception and
         * FatalError states. 
         *
         * In the first case, updateHook() is still called, and recover() allows
         * you to go back into the Running state.  In the second case, the
         * errorHook() will be called instead of updateHook(). In Exception, the
         * component is stopped and recover() needs to be called before starting
         * it again. Finally, FatalError cannot be recovered.
         */
        void updateHook();

        /** This hook is called by Orocos when the component is in the
         * RunTimeError state, at each activity step. See the discussion in
         * updateHook() about triggering options.
         *
         * Call recover() to go back in the Runtime state.
         */
        void errorHook();

        /** This hook is called by Orocos when the state machine transitions
         * from Running to Stopped after stop() has been called.
         */
        void stopHook();

        /** This hook is called by Orocos when the state machine transitions
         * from Stopped to PreOperational, requiring the call to configureHook()
         * before calling start() again.
         */
        void cleanupHook();
    };
}

#endif

