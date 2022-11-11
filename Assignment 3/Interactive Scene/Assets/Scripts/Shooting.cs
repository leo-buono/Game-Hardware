using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;

public class Shooting : MonoBehaviour
{
	public Transform shootOrigin;
	public InputAction shoot;
	public InputAction aim;
	public float range = 100f;
	public LayerMask mask = 1;
	public CamController cam;
	public Vector2 aimSens;
	public Vector3 aimPos = Vector3.right * 0.5f + Vector3.back * 2f;
	Vector2 defaultSens;
	Vector3 defaultPos;


	private void Awake() {
		shoot.started += ctx => {
			RaycastHit hit;
			if (Physics.Raycast(shootOrigin.position, shootOrigin.forward, out hit, range, mask)) {
				if (hit.transform.CompareTag("Finish"))
					hit.transform.gameObject.SetActive(false);
			}
		};
		defaultSens = cam.sensitivity;
		defaultPos = shootOrigin.localPosition;
		aim.performed += ctx => {
			cam.sensitivity = Vector2.Lerp(defaultSens, aimSens, ctx.ReadValue<float>());
			shootOrigin.localPosition = Vector3.Lerp(defaultPos, aimPos, ctx.ReadValue<float>());
		};
		aim.canceled += ctx => {
			cam.sensitivity = defaultSens;
			shootOrigin.localPosition = defaultPos;
		};
	}

	private void OnEnable() {
		shoot.Enable();
		aim.Enable();
	}

	private void OnDisable() {
		shoot.Disable();
		aim.Disable();
	}
}
