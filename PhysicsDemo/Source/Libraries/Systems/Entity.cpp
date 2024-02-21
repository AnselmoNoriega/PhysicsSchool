{
	auto lin_sim_view = registry.view<spatial2_component, linear_body2_component>();
	for (auto&& [entity, spatial, linear] : lin_sim_view.each())
	{
		math::vector2_f32 acceleration = Gravity2 + linear.applied_force * linear.inverse_mass;
		math::euler_integration(linear.velocity, acceleration, deltaTime);
		linear.velocity *= Damping;
		math::euler_integration(spatial.position, linear.velocity, deltaTime);
	}
		}
{
	auto lin_sim_view = registry.view<spatial3_component, linear_body3_component>();
	for (auto&& [entity, spatial, linear] : lin_sim_view.each())
	{
		math::vector3_f32 acceleration = Gravity + linear.applied_force * linear.inverse_mass;
		math::euler_integration(linear.velocity, acceleration, deltaTime);
		linear.velocity *= Damping;
		math::euler_integration(spatial.position, linear.velocity, deltaTime);
	}
}

template <typename T, typename V>
inline void euler_integration(V& value, V const& derivative, T delta_t)
{
	value += (delta_t * derivative);
}

template <typename T, typename V>
inline void midpoint_integration(V& value, V const& derivative, V const& last_derivative, T delta_t)
{
	value += (delta_t * (last_derivative + derivative) * 0.5f);
}