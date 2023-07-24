
export default defineNuxtRouteMiddleware((to, from) => {
  const jwt = useCookie('session-jwt');
  const hasJwt = !!jwt.value;
  if (!hasJwt && to.path !== "/login") {
    return navigateTo('/login')
  } else if (hasJwt && to.path === "/login") {
    return navigateTo('/');
  }
})
